/*
    gcc -O0 -fno-stack-protector -no-pie -o bof1 bof1.c
*/

#include <stdio.h>
#include <unistd.h>

void init(){
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
}

void win(){
    printf("You Win!\n");
}

void main(){
    init();
    char buf[512] = {};
    printf("buf > ");
    read(0, buf, 1024);
    printf("Your Data : %s", buf);
}

