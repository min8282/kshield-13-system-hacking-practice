/*
    gcc -O0 -fno-stack-protector -no-pie -z execstack bof2.c -o bof2
*/

#include <stdio.h>
#include <unistd.h>

void init(){
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
}

void main(){
    init();
    char buf[512] = {};
    printf("buf @ %p\n", buf);
    printf("buf > ");
    read(0, buf, 1024);
    printf("Your Data : %s\n", buf);
}

