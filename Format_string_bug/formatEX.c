#include <stdio.h>
// gcc ./formatEx.c -g -m32 -no-pie -o formatEx
int answer = 0x9047;
void main(){
    char buf[512]={0,};
    fgets(buf,51,stdin);    
    printf(buf);
    if(answer==0xbeefdead){
        printf("you win!\n");
    }
}