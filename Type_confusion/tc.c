#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


struct number {
    int64_t num; // pointer addr = 8bytes
};

struct string {
    char *string;
};

void printString(struct string *str){
    printf("%s\n", str->string);
}

int main(){
    struct number *number = malloc(sizeof(struct number));
    scanf("%ld\n", &number->num);
    printString(number);
    return 0;
}