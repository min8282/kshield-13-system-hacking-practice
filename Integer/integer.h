#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#define MAX_STORAGE_SIZE 64
#define MAX_KEY_LEN 16
#define MAX_STR_LEN 32

struct data{
    uint8_t * buf;
    size_t len;
};
struct data *storage = NULL;

struct input{
    uint8_t *key;
    uint8_t *str;
    size_t size;
};

int parseData(struct input *input);
struct input* getUserInput();
int init();
int save();
int menu();
