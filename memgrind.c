#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <sys/time.h>
#include <math.h>

#include <stdio.h>
#include <unistd.h> // for usleep


//TODO :: Implement test functions.

//Malloc and free() seem to work as expected.

#define END_PROGRAM 0

#define TRUE 1
#define FALSE 0

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
    printf("~---------------------------------------~\n");
    printf("|                Test One               |\n");
    printf("|---------------------------------------|\n");
    long long time = 0;
    for(int i = 0; i<50; i++) {
        long long start = current_microseconds();
        for(int i = 0; i<120; i++) {
            void * p = malloc(1);
            free(p);
        }
        long long end = current_microseconds();
        time += (end-start);
    }
    printf("|Total time   : %.2lf microseconds     |\n", (double)time);
    printf("|Average time : %.2lf microseconds       |\n", (double)(time)/50);
    printf("~---------------------------------------~\n");
}

void test2() {
    printf("~---------------------------------------~\n");
    printf("|                Test Two               |\n");
    printf("|---------------------------------------|\n");
    long long time = 0;
    for(int j = 0; j<50; j++) {
        void * arr [120];
        long long start = current_microseconds();
        for(int i = 0; i<120; i++) {
            void * p = malloc(1);
            arr[i] = p;    
        }
        // print_mem();
        for(int i = 0; i<120; i++) {
            free(arr[i]);
        }
        long long end = current_microseconds();
        time += (end-start);
    }
    // print_mem();
    printf("|Total time   : %.2lf microseconds      |\n", (double)time);
    printf("|Average time : %.2lf microseconds      |\n", (double)(time)/50);
    printf("~---------------------------------------~\n");
}
/**
 * Allocates arr[index] with a newly allocated pointer.
 * 
 * Returns index+1, that is, the new index to be allocated.
*/
int allocate(void ** arr, int index, int * allocations) {
    void * p = malloc(1);
    arr[index] = p;
    index++;
    (*allocations)++; //incremenet the number of allocations
    return index;
}
/**
 * Deallocates arr[index] if arr[index] is available to be deallocated.
 * 
 * That is that arr[index] must not be null, and index must be larger than or equal to 0.
 * 
 * Note that this method performs no error checking.
 * 
 * Returns either index (if the pointer could not be deallocated, or that the given index is 0), or index-1 (if the pointer could be deallocated)
*/
int deallocate(void ** arr, int index) {
    if(index >= 0 && arr[index] != NULL) {
        free(arr[index]);
        arr[index] = NULL;
    }
    return index;
}
/**
 * Fills the given array with the given size with NULL pointers.
*/
void nullify(void ** arr, int size) {
    for(int i = 0; i<size; i++) {
        arr[size] = NULL;
    }
}
void test3() {
    void * arr [120];
    nullify(arr, 120);
    int index = 0;
    int allocations = 0;

    long long time = 0;
    for(int i = 0; i<50; i++) {
        long long start = current_microseconds();
        while(allocations < 120) {
            int random = rand()%2;
            // printf("-----------\n");
            // printf("Index : %d\n", index);
            // printf("Allocations : %d\n", allocations);
            // printf("-----------\n");
            if(random == 0) {
                index = allocate(arr, index, &allocations);
            }
            else {
                index = deallocate(arr, index);
            }
        }
        while(index >= 0) {
            free(arr[index]);
            index--;
        }
        long long end = current_microseconds();
        time += (end - start);
        // print_mem();
    }
    if(leak()) {
        printf("Memory leak.\n");
    }
    else {
        printf("No memory leak :)\n");
    }
    printf("|Total time   : %.2lf microseconds      |\n", (double)time);
    printf("|Average time : %.2lf microseconds      |\n", (double)(time)/50);
    printf("~---------------------------------------~\n");

    
}

/**
 * Allocates 16 bytes of memory until malloc() fails.
 * Then, randomly frees the allocated pointers. This is a test of coelscence.
*/
void test4() {

    printf("~---------------------------------------~\n");
    printf("|               Test Four               |\n");
    printf("|---------------------------------------|\n");

    void * arr [128]; //128 chunks of 32 bytes (16 for meta data + 16 for size) fits within 4096 bytes
    void * p = malloc(24);
    int i = 0;
    while(p != NULL) {
        arr[i] = p;
        p = malloc(24);
        i++;
    }
    print_mem();
    i--; 
    while(i >= 0) { //frees all of the allocated pointers
        free(arr[i]);
        i--;
    }
    print_mem();
    printf("~---------------------------------------~\n");
    
}
void swap(int * arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}
int unique(int * arr, int size) {
    for(int i = 0; i<size; i++){
        for(int j = i+1; j<size; j++) {
            if(arr[i] == arr[j]) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
void generate(int arr[], int size) {
    for(int i = 0; i<size; i++) {
        arr[i] = i;
    }
    print(arr, size);
    if(unique(arr, size)) {
        printf("Generated set is unique\n");
    }
    else {
        printf("Generated set is not unique\n");
    }
    //shuffle the array to randomize it
    for(int i = 0; i<size; i++) {
        int random1 = rand()%size;
        int random2 = rand()%size;
        swap(arr, random1, random2);
    }
    print(arr, size);

    if(unique(arr, size)) {
        printf("Generated set is unique\n");
    }
    else {
        printf("Generated set is not unique\n");
    }
} 
void print(int * arr, int size) {
    for(int i = 0; i<size; i++) {
        printf("%d\t", arr[i]);
    }
    printf("\n");
}
void test5() {
    printf("~---------------------------------------~\n");
    printf("|               Test Five               |\n");
    printf("|---------------------------------------|\n");

    void * arr [128]; //128 chunks of 32 bytes (16 for meta data + 16 for size) fits within 4096 bytes
    void * p = malloc(24);
    int i = 0;
    while(p != NULL) {
        arr[i] = p;
        p = malloc(24);
        i++;
    }
    print_mem();
    int random [128]; //have a random set of 128 numbers
    generate(random, 128);
    print(random, 128);
    for(int j = 0; j<128; j++) {
        void * p = arr[random[j]];
        free(p);
    }

    print_mem();
    printf("~---------------------------------------~\n");
}

void test5() {

    int arr[200];
    void * p = malloc(1);
    int i = 0;
    while(p != NULL) {
        arr[i] = p;
        p = malloc(1);
        i++;
    }

    
}
/**
 * Slowly prints the given text to the terminal.
*/
void slowprint(char * text) {
    // int delay_ms = 100000;
    int delay_ms = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        putchar(text[i]);
        fflush(stdout); // Flush the output buffer to make text appear immediately
        usleep(delay_ms);
    }

    putchar('\n'); // Print a newline character at the end
}
/**
 * Performs the given test.
*/
void perform(int test) {
    switch(test) {
        case 1 : test1(); break;
        case 2 : test2(); break;
        case 3 : test3(); break;
        case 4 : test4(); break;
        case 5 : test5(); break;
        default : return;
    }
}

int choose() {
    int choice = -1;
    slowprint("*Select a test from the following:\n\n");
    slowprint("*Test (1) : malloc() and immediately free() a 1-byte object, 120 times\n\n");
    slowprint("*Test (2) : Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks\n\n");
    slowprint("*Test (3) : Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object (if any), until you have allocated 120 times. Deallocate any remaining objects\n\n");
    slowprint("*Test (4) : Allocate until there is no more memory left. Then, randomly free the allocated pointers until all are freed.\n");
    slowprint("*Exit (0) : Exit the program\n\n");
    slowprint("*Enter a number [0,3] :");
    scanf("%d", &choice);
    return choice;

}
int main() {
    printf("~---------------------------------------------------------------------------~\n");
    slowprint("Welcome to memgrind, the perfect place to test memory allocation functions!\n\n\n\n\n");

    int choice = choose();
    while(choice != END_PROGRAM) {
        perform(choice);
        choice = choose();
    }
}