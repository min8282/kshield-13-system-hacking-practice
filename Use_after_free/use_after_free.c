/*
    gcc use_after_free.c -no-pie -o use_after_free
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct strStruct{
    int idx;
    char* string;
};

struct numStruct{
    int idx;
    int64_t num;
};


struct strStruct *str = NULL;
struct numStruct *num = NULL;

void init() {
  setvbuf(stdin, 0, 2, 0);
  setvbuf(stdout, 0, 2, 0);
  setvbuf(stderr, 0, 2, 0);
}

int setString(){
    printf("[setString]\n");
    if(str == NULL){
        str = malloc(sizeof(struct strStruct));
        memset(str, 0, sizeof(struct strStruct));
    }
    printf("input str > ");
    char* buf = str->string;
    if(!buf){
        buf = malloc(0x10);
    }
    memset(buf, 0, 0x10);
    read(0, buf, 0x10);
    str->string = buf;
    return 0;
}

int getString(){
    printf("[getString]\n");
    if(str == NULL){
        return -1;
    }
    printf("string @ %s\n", str->string);
    return 0;
}

int delString(){
    printf("[delString]\n");
    if(str == NULL){
        return -1;
    }
    free(str->string);
    free(str);
    return 0;
}


int setNum(){
    printf("[setNum]\n");
    if(num == NULL){
        num = malloc(sizeof(struct numStruct));
        memset(num, 0, sizeof(struct numStruct));
    }
    printf("input num > ");
    scanf("%ld", &num->num);
    return 0;
}

int getNum(){
    printf("[getNum]\n");
    if(!num){
        return -1;
    }
    printf("num @ %ld\n", num->num);
    return 0;
}

int delNum(){
    printf("[delNum]\n");
    if(!num){
        return -1;
    }
    free(num);
    num = NULL;
    return 0;
}

int menu(){
    int idx = 0;
    printf("[MENU]\n");
    printf("1. setString\n");
    printf("2. getString\n");
    printf("3. delString\n");
    printf("4. setNum\n");
    printf("5. getNum\n");
    printf("6. delNum\n");
    printf("> ");
    scanf("%d",&idx);
    switch(idx){
        case 1:
            setString();
            break;
        case 2:
            getString();
            break;
        case 3:
            delString();
            break;
        case 4:
            setNum();
            break;
        case 5:
            getNum();
            break;  
        case 6:
            delNum();
            break;
        default:
            break;
    }
}

int flag = 0xdeadbeef;
int main(){
    init();
    while(1){
        printf("flag @ %x\n", flag);
        if(flag == 0x31337){
            printf("You Win!\n");
            return 0;
        }
        menu();
    }
}