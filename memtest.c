#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

int main() {
    void * p = malloc(0);
    print_mem();
    p = malloc(1);
    print_mem();
    p = malloc(1);
    print_mem();
}