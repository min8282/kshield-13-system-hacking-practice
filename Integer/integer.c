/*
    gcc ./integer.c -o integer
*/

#include "integer.h"

int init(){
    if(storage != NULL){
        printf("[-] Storage has already.\n");
        return -1;
    }
    storage = (struct data*)malloc(sizeof(struct data));
    storage->buf = (uint8_t*)malloc(MAX_STORAGE_SIZE);
    memset(storage->buf, 0, MAX_STORAGE_SIZE);
    storage->len = 0;
    printf("[+] Storage inited. @ %p\n", storage->buf);
    return 0;
}

int save(){
    struct input * input = getUserInput();
    if(input == NULL){
        return -1;
    }
    if(0 > parseData(input)){
        printf("[-] Input not saved.\n");
        return -1;
    }
    printf("[+] Input saved.\n");
    return 0;
}

void flushbuf(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

struct input* getUserInput(){
    if(storage == NULL){
        printf("[-] Storage is NULL.\n");
        return NULL;
    }
    struct input *input = (struct input*) malloc(sizeof(struct input));
    input->key = (uint8_t*)malloc(MAX_KEY_LEN);
    memset(input->key, 0, MAX_KEY_LEN);
    input->str = (uint8_t*)malloc(MAX_STR_LEN);
    memset(input->str, 0, MAX_STR_LEN);

    printf("input key >");
    fgets(input->key, MAX_KEY_LEN, stdin);
    size_t keySz = strlen(input->key) - 1;
    input->key[keySz] = '\0';

    printf("input str >");
    fgets(input->str, MAX_STR_LEN, stdin);
    size_t strSz = strlen(input->str) - 1;
    input->str[strSz] = '\0';        
    
    if(keySz == 0 || strSz == 0){
        printf("[-] Invalid input.\n");
        return NULL;
    }
    input->size = keySz + strSz;
    return input;
}

int parseData(struct input *input){
    size_t curSz = storage->len;
    size_t inputSz = input->size;

    size_t keySz = strlen(input->key);
    size_t strSz = strlen(input->str);

    if (inputSz > MAX_STORAGE_SIZE - 2 - curSz){
        printf("[-] Input too Large\n");
        return -1;
    }

    storage->buf[curSz++] = ',';
    memcpy(storage->buf + curSz, input->key, keySz);
    curSz += keySz;

    storage->buf[curSz++] = '=';
    memcpy(storage->buf + curSz, input->str, strSz);
    curSz += strSz;
    
    storage->buf[curSz] = '\0';
    storage->len = curSz;

    free(input->key);
    free(input->str);
    free(input);
    return 0;
}

int menu(){
    int idx = 0;
    printf("[MENU]\n");
    printf("1. Init\n");
    printf("2. Save\n");
    printf("3. Exit\n");
    scanf("%d",&idx);
    flushbuf();
    switch(idx){
        case 1:
            init();
            break;
        case 2:
            save();
            break;
        case 3:
            printf("[-] Good Bye~\n");
            exit(0);
        default:
            break;
    }
}

int main(){
    printf("This Data Storage System.\n\n");
    while(1){
        if(storage){
            printf("[!] Storage size : %lu\n", storage->len);
        }
        menu();
    }
}