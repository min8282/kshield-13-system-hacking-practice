// gcc -o rtl -fno-stack-protector -no-pie rtl.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int init() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);

    return 0;
}

int vuln() {
    printf("%p\n", read);

    char buf[0x100] = {0, };

    read(0, buf, 0x200);

    return 0;
}

int main() {
    init();

    vuln();

    return 0;
}
