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
int main() {
    test1();


}