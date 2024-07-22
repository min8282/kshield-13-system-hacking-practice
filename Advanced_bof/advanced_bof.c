/*
    gcc -O0 -fno-stack-protector -no-pie -z execstack advanced_bof.c -o advanced_bof
*/

#include <stdio.h>
#include <unistd.h>

void init(){
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
}

void vuln(){
    char buf[512] = {};
    printf("buf @ %p\n", buf);
    printf("buf > ");
    read(0, buf, 520);
    printf("Your Data : %s\n", buf); 
}

void main(){
    int makeSFP = 0;
    init();
    vuln();
}

