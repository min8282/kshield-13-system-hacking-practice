#include "util.h"

void debug() {
    puts("[!] Paused...");
    getchar();
}

int new_queue() {
    return msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
}

int del_queue(int msg_id) {
    return msgctl(msg_id, IPC_RMID, NULL);
}

int send_msg(int msg_id, long msg_type, const char *buf, size_t len) {
    size_t msg_size = sizeof(long) + len;
    Msg *msg = (Msg *) malloc(msg_size);
    memset(msg, 0x00, msg_size);

    msg->type = msg_type;
    memcpy(msg->text, buf, len);

    int res = msgsnd(msg_id, msg, len, 0);

    free(msg);

    return res;
}

int recv_msg(int msg_id, long msg_type, char *buf, size_t len) {
    size_t msg_size = sizeof(long) + len;
    Msg *msg = (Msg *) malloc(msg_size);
    memset(msg, 0x00, msg_size);

    msg->type = msg_type;
    
    int res = msgrcv(msg_id, msg, len, 1, IPC_NOWAIT | MSG_COPY | MSG_NOERROR);

    memcpy(buf, msg->text, len);
    free(msg);

    return res;
}