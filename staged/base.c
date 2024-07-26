#include "util.h"

#define _GNU_SOURCE

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

    int fd = fsopen("ext4", 0);
    if (fd < 0) {
	    perror("fsopen");
        exit(1);
    }
    printf("[*] fd @ %d\n", fd);

    fsconfig(fd, FSCONFIG_SET_STRING, "key", "value", 0);

    debug();
    
    return 0;
}