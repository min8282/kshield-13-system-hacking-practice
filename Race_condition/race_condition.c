/*
    gcc race_condition.c -o race_condition
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT (8888)
#define BUFFER_SIZE (1024)

#define send_msg(sock, msg) (send(sock, msg, strlen(msg), 0))

typedef struct {
    int id;
    int balance;
    int is_used;
} Coupon;

typedef struct {
    int id;
    char name[24];
    int balance;
    Coupon coupons[5];
} Account;
Account accounts[10];
int account_cur = 0;

int receive_data(int sock, char *buf) {
    memset(buf, 0, BUFFER_SIZE);
    return recv(sock, buf, BUFFER_SIZE - 1, 0);
}

int receive_int(int sock, char *buf) {
    receive_data(sock, buf);
    return atoi(buf);
}

int receive_string(int sock, char *buf) {
    receive_data(sock, buf);
    return strlen(buf);
}

void print_menu(int sock) {
    send_msg(sock, "1. Create account\n");
    send_msg(sock, "2. View account\n");
    send_msg(sock, "3. Use coupon\n");
    send_msg(sock, "4. Buy item\n");
    send_msg(sock, "5. Exit\n");
    send_msg(sock, "> ");
}

void print_accounts(int sock){
    char buf[128] = {};
    send_msg(sock, "[Accounts]\n");
    if(account_cur){
        for(int i = 0; i < account_cur; i++){
            sprintf(buf, "[%d]", i);
            send_msg(sock, buf);
        }
    }
    send_msg(sock, "\n");
}

void create_account(int sock, char *buf) {
    if (account_cur >= 10) {
        send_msg(sock, "Too many accounts...\n");
        return;
    }

    send_msg(sock, "Name> ");
    int n = receive_string(sock, buf);
    n = n < 24 ? n : 23;

    accounts[account_cur].id = account_cur;
    strncpy(accounts[account_cur].name, buf, n);
    accounts[account_cur].balance = 0;
    for (int i = 0; i < 5; i++) {
        accounts[account_cur].coupons[i].id = i;
        accounts[account_cur].coupons[i].balance = 5000;
        accounts[account_cur].coupons[i].is_used = 0;
    }

    account_cur++;

    send_msg(sock, "Successfully created!\n\n");
}

void view_account(int sock, char *buf) {
    if (!account_cur){
        send_msg(sock,"Account not Exists..\n\n");
        return;
    }
    send_msg(sock, "Account> ");
    int account_id = receive_int(sock, buf);
    if (account_id >= account_cur) {
        send_msg(sock, "Invalid account...\n\n");
        return;
    }

    char tmp[1024] = {};
    sprintf(tmp,"\nName:%sBalance : %d\n\n",accounts[account_id].name, accounts[account_id].balance);
    send_msg(sock, tmp);
}

void use_coupon(int sock, char *buf) {
    send_msg(sock, "Account> ");
    int account_id = receive_int(sock, buf);
    if (account_id >= account_cur) {
        send_msg(sock, "Invalid account...\n\n");
        return;
    }

    send_msg(sock, "Coupon[0-4]> ");
    int coupon_id = receive_int(sock, buf);
    if (0 > coupon_id || coupon_id >= 5) {
        send_msg(sock, "Invalid coupon...\n\n");
        return;
    }

    if (accounts[account_id].coupons[coupon_id].is_used) {
        send_msg(sock, "Already used coupon...\n\n");
        return;
    }

    accounts[account_id].balance += accounts[account_id].coupons[coupon_id].balance;
    sleep(1);
    accounts[account_id].coupons[coupon_id].is_used = 1;

    send_msg(sock, "Successfully used!\n\n");
}

void buy_item(int sock, char *buf) {
    send_msg(sock, "Account> ");
    int account_id = receive_int(sock, buf);
    if (account_id >= account_cur) {
        send_msg(sock, "Invalid account...\n\n");
        return;
    }

    if (accounts[account_id].balance < 50000) {
        send_msg(sock, "Insufficient balance...\n\n");
        return;
    }

    accounts[account_id].balance -= 50000;
    send_msg(sock, "You Win!\n\n");
    sleep(3);
    exit(0);
}

void *connection_handler(void *args) {
    int sock = *(int *)args;
    char buf[BUFFER_SIZE];

    while (1) {
        print_accounts(sock);
        print_menu(sock);
        int menu = receive_int(sock, buf);

        switch (menu) {
        case 1:
            create_account(sock, buf);
            break;

        case 2:
            view_account(sock, buf);
            break;

        case 3:
            use_coupon(sock, buf);
            break;

        case 4:
            buy_item(sock, buf);
            break;

        case 5:
            send_msg(sock, "Bye~\n");
            goto ESCAPE;

        default:
            send_msg(sock, "Invalid menu...\n\n");
            break;
        }
    }

    ESCAPE:

    close(sock);

    return NULL;
}

void init() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);

    signal(SIGPIPE, SIG_IGN);
}

int main() {
    init();

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_sockaddr, clnt_sockaddr;
    socklen_t clnt_sockaddr_len = sizeof(clnt_sockaddr);
    pthread_t th;

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_sockaddr.sin_family = AF_INET;
    serv_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_sockaddr.sin_port = htons(PORT);

    if (bind(serv_sock, (struct sockaddr*)&serv_sockaddr, sizeof(serv_sockaddr)) != 0) {
        perror("bind");
        exit(1);
    }

    if (listen(serv_sock, 0) != 0) {
        perror("listen");
        exit(1);
    }

    while (1) {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_sockaddr, &clnt_sockaddr_len);
        if (clnt_sock == -1) {
            perror("accept");
            break;
        }

        if (pthread_create(&th, NULL, connection_handler, (void*)&clnt_sock) != 0) {
            perror("pthread_create");
            close(clnt_sock);
            break;
        }

        clnt_sockaddr_len = sizeof(clnt_sockaddr);
    }

    close(serv_sock);

    return 0;
}