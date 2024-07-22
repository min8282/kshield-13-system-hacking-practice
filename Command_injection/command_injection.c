/*
    gcc ./command_injection.c -o command_injection
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void init(){
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

int filter(char *cmd){
    int len = strlen(cmd);
    for(int i = 0; i < len ; i++){
        if(cmd[i] == '$' || cmd[i] == '&' || cmd[i] == ';' || cmd[i] == '|')
            return 0;
    }
    if(strncmp(cmd, "ls", 2) == 0 || strncmp(cmd, "whoami", 6) == 0){
        return 1;
    }
    return 0;
}

void main(){
    char cmd[31] = {};
    init();
    printf("command >");
    read(0, cmd, 30);
    if(filter(cmd)){
        system(cmd);
    }else{
        printf("[-] Filtered\n");
    }
}
