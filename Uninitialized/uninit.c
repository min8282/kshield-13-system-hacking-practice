/*
    gcc ./uninit.c -o uninit
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_COUNT 10

struct Data *allocates[ALLOC_COUNT] = {};

struct Data{
    uint8_t * ptr;
    uint64_t size;
};


void flushbuf(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

uint32_t findEmpty(){
    for(int i = 0; i < ALLOC_COUNT; i++){
        if(allocates[i] == NULL){
            return i;
        }
    }
    return -1;
}

void allocate(){
    int idx = findEmpty();
    if(idx < 0){
        printf("[-] No empty space\n");
        return;
    }
    struct Data * data = (struct Data*)malloc(sizeof(struct Data));
    allocates[idx] = data;
    printf("[+] index [%d] Allocated at %p\n",idx, data);
    return;
}

void insert(){
    uint32_t idx = 0;
    char buf[1024] = {};

    printf("select idx > ");
    scanf("%u",&idx);
    flushbuf();

    if(idx > 9 || allocates[idx] == NULL){
        printf("[-] Invalid index.\n");
        return;
    };
    
    struct Data * data = allocates[idx];
    if(data->ptr != NULL){
        printf("[-] Data Exists.\n");
        return;
    }

    printf("input > ");
    fgets(buf, 1024, stdin);
    uint32_t len = strlen(buf);
    buf[len - 1] = '\0';

    data->ptr = (uint8_t*)malloc(len);
    memcpy(data->ptr, buf, len);
    data->size = len;
    printf("[+] Data Inserted at [%d]\n", idx);
    return;
}

void readData(){
    uint32_t idx = 0;

    printf("select idx > ");
    scanf("%u",&idx);
    flushbuf();

    if(idx > 9 || allocates[idx] == NULL){
        printf("[-] Invalid index.\n");
        return;
    };
    
    struct Data * data = allocates[idx];
    if(data->ptr == NULL){
        printf("[-] Data not Exists.\n");
        return;
    }
    printf("Data > %s\n\n", data->ptr);
    return;
}

void deallocate(){
    uint32_t idx = 0;

    printf("select idx > ");
    scanf("%u",&idx);
    flushbuf();

    if(idx > 9 || allocates[idx] == NULL){
        printf("[-] Invalid index.\n");
        return;
    };
    
    struct Data * data = allocates[idx];
    if(data->ptr != NULL){
        free(data->ptr);
    }
    free(data);
    allocates[idx] = NULL;
    printf("[+] [%d] deallocated\n", idx);
    return;
}

void printSpace(){
    printf("\n[Space]\n");
    for(int i = 0 ; i < ALLOC_COUNT; i++){
         if(allocates[i] != NULL){
            printf("[%d] ", i);
        }
    }
    printf("\n");
    printf("\n");
}


int menu(){
    printSpace();

    printf("[MENU]\n");
    printf("1. Allocate\n");
    printf("2. Insert\n");
    printf("3. Read\n");
    printf("4. Deallocate\n");
    printf("0. Exit\n");
    printf("> ");

    int idx = 0;
    scanf("%u",&idx);
    flushbuf();
    switch(idx){
        case 1:
            allocate();
            break;
        case 2:
            insert();
            break;
        case 3:
            readData();
            break;
        case 4:
            deallocate();
            break;
        case 0:
            exit(0);
            break;
        default:
            break;
    }
}

int main(){
    printf("Hello. This is Simple Allocator.\n\n");
    while(1){
        menu();
    }
}