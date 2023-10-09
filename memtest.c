#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


void test1() {

    void * arr[200];
    void * p = malloc(1);
    int i = 0;
    while(p != NULL) {
        arr[i] = p;
        p = malloc(1);
        i++;
    }
    printf("Allocated %d times before OOM\n", i);
}
void test0() {
    printf("~--------------~\n");
    printf("|  Test Zero   |\n");
    printf("~--------------~\n");

    printf("********************************\n");
    printf("Allocating eight byte of memory\n");
    printf("********************************\n");

    void * p = malloc(8);
    print_mem();
    printf("********************************\n");
    printf("Allocating eight bytes of memory\n");
    printf("********************************\n");
    void *p2 = malloc(8);

    print_mem();
    printf("********************************\n");
    printf("Allocating eight bytes of memory\n");
    printf("********************************\n");

    void * p3 = malloc(8);
    print_mem();


    printf("********************************\n");
    printf("Freeing second allocated pointer\n");
    printf("********************************\n");

    free(p2);

    print_mem();
    
    printf("********************************\n");
    printf("Freeing third allocated pointer\n");
    printf("********************************\n");

    free(p3);

    print_mem();

    printf("********************************\n");
    printf("Freeing first allocated pointer\n");
    printf("********************************\n");
    free(p);
    print_mem();
}

void testa() {
    void * p = malloc(8);
    // void * p2 = malloc(8);
    print_mem();
    free(p);
    print_mem();
}
int main() {
    test0();
}