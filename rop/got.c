// gcc -o got -no-pie got.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int unused() {
    system("ls");

    return 0;
}

int init() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);

    return 0;
}

int vuln() {
    char name[24] = {0, };

    scanf("%23s", name);

    uint64_t addr;

    scanf("%ld", &addr);
    scanf("%ld", addr);

    puts(name);

    return 0;
}

int main() {
    init();

    vuln();

    return 0;
}
