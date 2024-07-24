// gcc -o canary -no-pie canacy.c

#include <stdio.h>
#include <stdlib.h>

int get_shell() {
    execve("/bin/sh", NULL, NULL);

    return 0;
}

int init() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);

    return 0;
}

int vuln() {
    char buf[0x100] = {0, };

    read(0, buf, 0x200);
    printf("%s\n", buf);
    read(0, buf, 0x200);

    return 0;
}

int main() {
    init();

    vuln();

    return 0;
}
