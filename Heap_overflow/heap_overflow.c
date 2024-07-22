/*
    gcc ./heap_overflow.c -no-pie -o heap_overflow
*/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_COUNT 10

struct Data *allocates[ALLOC_COUNT] = {0,};

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
    memset(data, 0, sizeof(struct Data));
    allocates[idx] = data;
    printf("%p\n", data);
    printf("[+] Allocated at %p\n", data);
    return;
}

#define BUF_SIZE 1024

void insert(){
    uint32_t idx = 0;
    char buf[BUF_SIZE] = {0,};

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
    int len = read(0, buf, BUF_SIZE);
    data->ptr = (uint8_t*)malloc(len);
    memcpy(data->ptr, buf, len);
    data->size = len;
    
    printf("[+] Data Inserted!\n");
    return;
}

void modify(){
    uint32_t idx = 0;
    char buf[BUF_SIZE] = {0,};

    printf("select idx > ");
    scanf("%u",&idx);
    flushbuf();

    if(idx > 9 || allocates[idx] == NULL){
        printf("[-] Invalid index.\n");
        return;
    };
    
    struct Data * data = allocates[idx];
    if(data->ptr == NULL){
        printf("[-] Data not exists.\n");
        return;
    }

    printf("input > ");
    read(0, data->ptr, BUF_SIZE);
    printf("[+] Data modified!\n");
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
    printf("[+] Data : %s\n\n", data->ptr);
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
    printf("[+] Deleted!\n");
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
    printf("1. Create\n");
    printf("2. Insert\n");
    printf("3. Modify\n");
    printf("4. Read\n");
    printf("5. Delete\n");
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
            modify();
            break;
        case 4:
            readData();
            break;
        case 5:
            deallocate();
            break;
        case 0:
            exit(0);
            break;
        default:
            break;
    }
}

void init(){
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
}

uint32_t flag = 0xdeadbeef;

int main(){
    init();
    printf("Hello. This is Simple Allocator 2.\n");
    while(1){
        if(flag == 0x31337){
            printf("[!] Oops! You Win!\n");
            exit(0);
        }
        menu();
    }
}