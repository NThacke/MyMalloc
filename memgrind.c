#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <sys/time.h>
#include <math.h>


//TODO :: Implement test functions.

//Malloc and free() seem to work as expected.

/**
 * Returns the number of microseconds since EPOCH time.
*/
long long current_microseconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long microseconds = (long long)tv.tv_sec * 1000000 + tv.tv_usec;
    return microseconds;
}

void test0() {
    void * p = malloc(1);
    print_mem();
    free(p);
    print_mem();
}
void test1() {

    // long long time = 0;
    for(int i = 0; i<50; i++) {
        // long long start = current_microseconds();
        for(int i = 0; i<120; i++) {
            void * p = malloc(1);
            free(p);
        }
        // long long end = current_microseconds();
        // time += (end-start);
        // printf("%lld\n", (end-start));
    }
    // printf("Total time : %lld\n", time);
    // printf("Average time : %lf\n", (double)(time)/50);
}

void test2() {
    void * arr [120];
    for(int i = 0; i<120; i++) {
        void * p = malloc(1);
        arr[i] = p;    
    }
    print_mem();
    for(int i = 0; i<120; i++) {
        free(arr[i]);
    }
    print_mem();
}

void test3() {
    void * arr [120];

    
}

/**
 * Allocates 16 bytes of memory until malloc() fails.
 * Then, randomly frees the allocated pointers. This is a test of coelscence.
*/
void test4() {
    void * arr [128]; //128 chunks of 32 bytes (16 for meta data + 16 for size) fits within 4096 bytes
    void * p = malloc(24);
    int i = 0;
    while(p != NULL) {
        arr[i] = p;
        p = malloc(24);
        i++;
    }
    print_mem();

    // int j = 0;
    // void * q = arr[j];
    // while(q != NULL) {
    //     free(q);
    //     j++;
    //     q = arr[j];
    // }
    for(int j = 0; j<128; j++) {
        int random = rand();
        random = random%128;
        printf("The index is %d\n", random);
        void * choice = arr[random];
        while(choice == NULL) { //every iteration is guaranteed to free some pointer
            random++;
            choice = arr[random];
        }
        free(choice);
        arr[random] = NULL;
    }
    print_mem();
    
    
}
int main() {
    test2();
}