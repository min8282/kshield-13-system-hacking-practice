/*
    gcc ./type_confusion.c -o type_confusion
*/

#include "type_confusion.h"

void flushbuf(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int searchEmpty(){
    for(int i = 0; i < TOWN_SIZE; i++){
        if(town[i] == NULL){
            return i;
        }
    }
    return -1;
}

void makePerson(){
    int idx = searchEmpty();
    if(idx < 0){
        printf("[-] Town is full.\n");
        return;
    }
    
    struct person * person = (struct person*)malloc(sizeof(struct person));

    printf("Name > ");
    uint8_t *name = (uint8_t*)malloc(NAME_SIZE);
    fgets(name, NAME_SIZE, stdin);
    name[strlen(name) - 1] = '\0';
    person->name = name;

    printf("Job > ");
    uint8_t *job = (uint8_t*)malloc(JOB_SIZE);
    fgets(job, JOB_SIZE, stdin);
    job[strlen(job) - 1] = '\0';

    printf("Age > ");
    uint64_t age;
    scanf("%ld",&age);
    flushbuf();

    uint64_t type;
    if(age > 18){
        type = ADULT;
    }else{
        type = CHILD;
    }

    if(type == ADULT){
        person->type.adult.job = job;
        person->type.adult.age = age;
    }else if(type == CHILD){
        person->type.child.job = job;
        person->type.child.age = age;
    }

    person->typeflag = type;
    town[idx] = person;
    printf("[+] %s created at %p\n",person->name, person);
    return;
}

void upAge(){
    int idx;
    printf("idx in town> ");
    scanf("%d",&idx);
    flushbuf();

    if(idx > TOWN_SIZE - 1){
        printf("[-] invalid index.");
        return;
    }
    struct person *person = town[idx];
    if(person == NULL){
        printf("[-] invalid person.");
        return;
    }

    if(person->typeflag == ADULT){
        person->type.adult.age++;
    }else if(person->typeflag == CHILD){
        person->type.child.age++;
    }

    return;
}

void downAge(){
    int idx;
    printf("idx in town> ");
    scanf("%d",&idx);
    flushbuf();

    if(idx > TOWN_SIZE - 1){
        printf("[-] invalid index.");
        return;
    }
    struct person *person = town[idx];
    if(person == NULL){
        printf("[-] invalid person.");
        return;
    }

    if(person->typeflag == ADULT){
        person->type.adult.age--;
    }else if(person->typeflag == CHILD){
        person->type.child.age--;
    }

    return;
}

void transform(){
    int idx;
    printf("idx in town> ");
    scanf("%d",&idx);
    flushbuf();

    if(idx > TOWN_SIZE - 1){
        printf("[-] invalid index.");
        return;
    }

    struct person *person = town[idx];
    if(person == NULL){
        printf("[-] invalid person.");
        return;
    }

    printf("New Name >");
    memset(person->name, 0, NAME_SIZE);
    fgets(person->name, NAME_SIZE, stdin);
    person->name[strlen(person->name) - 1] = '\0';

    uint8_t *job = NULL;
    if(person->typeflag == ADULT){
        job =  person->type.adult.job;
    }else if(person->typeflag == CHILD){
        job = person->type.child.job;
    }

    memset(job, 0, JOB_SIZE);
    printf("New Job > ");
    fgets(job, JOB_SIZE, stdin);
    job[strlen(job) - 1] = '\0';

    if(person->typeflag == CHILD){
        if(person->type.child.age > 18){
            person->typeflag = ADULT;
        }
    }
    return;
}

void deletePerson(){
    int idx;
    printf("idx in town> ");
    scanf("%d",&idx);
    flushbuf();

    if(idx > TOWN_SIZE - 1){
        printf("[-] invalid index.");
        return;
    }
    struct person *person = town[idx];
    if(person == NULL){
        printf("[-] invalid person.");
        return;
    }
    free(person->name);

    if(person->typeflag == ADULT){
        free(person->type.adult.job);
    }else if(person->typeflag == CHILD){
        free(person->type.child.job);
    }
    free(person);
    town[idx] = NULL;
    return;
}

void printTown(){
    printf("\n[TOWN]\n");
    struct person *person;
    for(int i = 0; i < TOWN_SIZE; i++){
        if(town[i] != NULL){
            person = town[i];
            if(person->typeflag == ADULT){
                printf("[%d] %s(%s) \n", i, person->name, "Adult");
            }else if(person->typeflag == CHILD){
                printf("[%d] %s(%s) \n", i , person->name, "Child");
            }
        }
    }
    printf("\n");
}

int menu(){
    printTown();
    printf("[MENU]\n");
    printf("1. Make Person\n");
    printf("2. Age Up\n");
    printf("3. Age Down\n");
    printf("4. Transform\n");
    printf("5. Kill\n");
    printf("6. Leave\n");
    printf("> ");

    int idx = 0;
    scanf("%d",&idx);
    flushbuf();
    switch(idx){
        case 1:
            makePerson();
            break;
        case 2:
            upAge();
            break;
        case 3:
            downAge();
            break;
        case 4:
            transform();
            break;  
        case 5:
            deletePerson();
            break;
        case 6:
            printf("[-] Good Bye.\n");
            exit(0);
            break;
        default:
            break;
    }
}

int main(){
    printf("Hello. This is God Simulator.\n\n");
    while(1){
        menu();
    }
}