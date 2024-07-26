#include "util.h"

int main() {
    int msg_qid = new_queue();
    int msg_type = 1;

    char buf[] = "Hello";
    send_msg(msg_qid, msg_type, buf, strlen(buf));
    printf("[*] Send message @ %s\n", buf);

    memset(buf, 0x00, sizeof(buf));
    printf("[*] Clear buffer @ %s\n", buf);

    recv_msg(msg_qid, msg_type, buf, sizeof(buf));
    printf("[*] Receive message @ %s\n", buf);

    del_queue(msg_qid);

    return 0;
}