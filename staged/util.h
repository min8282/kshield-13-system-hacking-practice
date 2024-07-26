#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sched.h>
#include <pthread.h>
#include <poll.h>
#include <assert.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <linux/sched.h>

#ifndef __NR_fsconfig
#define __NR_fsconfig (431)
#endif

#ifndef __NR_fsopen
#define __NR_fsopen (430)
#endif

#define FSCONFIG_SET_STRING (1)

#define fsopen(name, flags) syscall(__NR_fsopen, name, flags)
#define fsconfig(fd, cmd, key, value, aux) syscall(__NR_fsconfig, fd, cmd, key, value, aux)

void debug();

typedef struct _Msg {
    long type;
    char text[1];
} Msg;

int new_queue();
int del_queue(int msg_id);
int send_msg(int msg_id, long msg_type, const char *buf, size_t len);
int recv_msg(int msg_id, long msg_type, char *buf, size_t len);