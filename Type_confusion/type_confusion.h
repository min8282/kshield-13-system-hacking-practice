#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct person{
    uint8_t * name;
    uint64_t typeflag;
    union{
        struct adult{
            uint64_t age;
            uint8_t * job;
        } adult;

        struct child{
            uint8_t * job;
            uint64_t age;
        } child;
    }type;
};


#define TOWN_SIZE 16
struct person * town[TOWN_SIZE] = {};

#define CHILD 1
#define ADULT 2
#define NAME_SIZE 0x10
#define JOB_SIZE 0x10