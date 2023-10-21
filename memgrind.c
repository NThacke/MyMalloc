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
 * Allocates a 1 byte object and stores the pointer to it at arr[index]
 * 
 * If successful, returns (index+1);
 * If unsuccessful, returns index.
 * 
 * Note that this method is only unssuccesful if index < 0 || index >= 120
*/
int alloc(void ** arr, int index, int * allocations) {
    if(index >= 0 && index < 120) {
        void * p = malloc(1);
        arr[index] = p;
        index++;
        (*(allocations))++;
    }
    return index;
}

/**
 * Deallocates the pointer located at arr[index].
 * 
 * If arr[index] can not be deallocated (it has either already been deallocated or not allocated -- the latter being only if dealloc() is invoked prior to alloc()),
 * this method will return the current index.
 * 
 * If the given index is -1, that is, dealloc() was invoked prior to alloc, this method will not free anything and return 0.
*/
int dealloc(void ** arr, int index) {
    if(index >= 0 && index < 120 && arr[index] != NULL) {
        void * p = arr[index];
        free(p);
        arr[index] = NULL;
    }
    if(index < 0) {
        return 0;
    }
    if(index > 120) {
        return 120;
    }
    return index;
}

void dealloc_cheap(void ** arr, int index) {
    for(int i = 0; i<index; i++) {
        void * p = arr[i];
        if(p != NULL) {
            free(p);
            arr[i] = NULL;
        }
    }
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
        arr[i] = NULL;
    }
}
void test_3() {
    printf("+---------------------------------------+\n");
    printf("|               Test Three              |\n");
    printf("+---------------------------------------+\n");

    long long time = 0;
    for(int i = 0; i<50; i++) {
        void * arr[120];
        nullify(arr, 120);
        int allocations = 0;
        int index = 0;
        long long start = current_microseconds();
        while(allocations < 120) { //Note that index is the index to be allocated, that is, it is guaranteed to store a NULL pointer.
            int random = rand()%2;
            if(random == 0) {
                index = alloc(arr, index, &allocations);
            }
            else {
                index = dealloc(arr, index-1);
            }
        }
        for(int i = 0; i<120; i++) { //frees everything that isn't already freed
            void * p = arr[i];
            if(p != NULL) {
                free(p);
            }
        }
        long long end = current_microseconds();
        time += (end-start);
    }
    printf("|Total time   : %.2lf microseconds     |\n", (double)time);
    printf("|Average time : %.2lf microseconds      |\n", (double)(time)/50);
    printf("+---------------------------------------+\n");

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
            if(arr[index] != NULL) {
                free(arr[index]);
            }
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
 * Test four: General stress test of malloc/free
 * Step 1: Generate random payload size
 * Step 2: Allocate block
 * Step 3 free
 * Step 4: Repeat this 50 times
*/

void test4() {
    printf("\n\n+---------------------------------------+\n");
    printf("|               Test Four               |\n");
    printf("+---------------------------------------+\n\n\n");


    // Start of iterations
    srand(time(NULL));

    // Start timer
    long long total_test_time = 0;
    for (int x = 0; x < 50; x++) {
        // Start iteration timer
        long long iteration_start = current_microseconds();
        // Step 1: Generate random payload size
        
        // print message 
        printf("Iteration %d of 50\n", x+1);

        // Note: We use srand here because we are not feeding into an array
        // To generate range: rand() % (max_number + 1 - minimum_number) + minimum_number
        int payload_size = rand() % (4064 + 1 - 1) + 1;
        printf("Payload size: %d\n", payload_size);

        // malloc and free immediately
        void * p = malloc(payload_size);
        free(p);

        // report timer
        long long iteration_end = current_microseconds();
        printf("Iteration time: %.2lf microseconds\n\n", (double)(iteration_end - iteration_start));
        // add to total timer
        total_test_time += (iteration_end - iteration_start);
    }

    printf("Test time: %.2lf microseconds\n", (double)(total_test_time));
    printf("Avg itme: %.2lf microseconds\n\n", (double)(total_test_time/50));
    printf("---------------------------------------\n\n\n");

}

/**
 * Allocates 16 bytes of memory until malloc() fails.
 * Then, randomly frees the allocated pointers. This is a test of coelscence.
 * We are skipping this performance test for now. dco43 has renamed the test for this


void testCoelscence() {

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
*/





/**
 * Test five: Performance testing of free(). 
 * Generate 128 chunks and fill up memory
 * Generate array with 128 random numbers (this corresponds to index)
 * call free() and free on random index generated in previous step
*/

void test5() {
    printf("~---------------------------------------~\n");
    printf("|               Test Five               |\n");
    printf("|---------------------------------------|\n");

    void * arr [127]; //128 chunks of 32 bytes (16 for meta data + 16 for size) fits within 4096 bytes
    void * p = malloc(16);
    int i = 0;
    while(p != NULL) {
        printf("allocating at %d\n", i);
        arr[i] = p;
        p = malloc(16);
        i++;
    }
    print_mem();
    int random [127]; //have a random set of 128 numbers
    generate(random, 127);
    print(random, 127);
    for(int j = 0; j<127; j++) {
        printf("%d\n", j);
        void * p = arr[random[j]];
        free(p);
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

/**
 * This test is a correctness test.
 * 
 * This method will allocate 120 1 byte pointers, and populate the payload with an integer.
 * 
 * Then, this method will iterate over the pointers, ensuring that each payload contains the correct data.
 * 
 * 
*/
void test6() {
    printf("----------------------------\n");
    printf("|         Test Six          |\n");
    printf("----------------------------\n");
    
    long long time = 0;
    for(int j = 0; j<50; j++) {
        void * arr [120];

        long long start = current_microseconds();
        for(int i = 0; i<120; i++) {
            void * p = malloc(1);
            arr[i] = p;
            
            //populate the payload
            *(int *)(p) = i; //the ith pointer contains a payload of i
        }

        //ensure that each pointer contains the correct data
        for(int i = 0; i<120; i++) {
            void * p = arr[i];
            if( *(int *)(p) != i) {
                //this is bad, we are overwrite some data!
                printf("Test6 failed, overwrote data!\n");
                return;
            }
        }
        for(int i = 0; i<120; i++) {
            free(arr[i]);
        }
        long long end = current_microseconds();
        time += (end-start);
    }
    printf("Test 6 was successful, no data had been overwritten!\n");
    printf("|Total time   : %.2lf microseconds     |\n", (double)time);
    printf("|Average time : %.2lf microseconds       |\n", (double)(time)/50);
    printf("~---------------------------------------~\n");

}
/**
 * Allocates 120 pointers and tests to see if any of the pointers overlap / are the same pointer. This is a test of malloc() to see if malloc() successfully returns unique pointers (other than NULL).
*/
void test7() {

    long long time = 0;
    for(int j = 0; j<50; j++) {
        void * arr[120];
        long long start = current_microseconds();
        for(int i = 0; i<120; i++) {
            void * p = malloc(1);
            arr[i] = p;
        }

        //O(n^2) test to see if duplicates. O(n) exists and requires hashmap implementation. This is unneccesary for our purposes here.

        for(int i = 0; i<120; i++) {
            void * p = arr[i];
            for(int k = i+1; k<120; k++) {
                void * q = arr[k];
                if(p == q) {
                    printf("Test (7) failed. Pointer %p and pointer %p point to the same payload but are different allocations\n.", p, q);
                    return;
                }
            }
            free(p); //we can free p, we won't check this pointer again (as it has been tested against all other pointers)
        }

        long long end = current_microseconds();
        time += (end-start);
    }
    printf("Test 7 was successful, no pointers had been malloced more than once.\n");
    printf("|Total time   : %.2lf microseconds     |\n", (double)time);
    printf("|Average time : %.2lf microseconds       |\n", (double)(time)/50);
    printf("~---------------------------------------~\n");
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
        case 3 : test_3(); break;
        case 4 : test4(); break;
        case 5 : test5(); break;
        case 6 : test6(); break;
        case 7 : test7(); break;
        default : return;
    }
}

int choose() {
    int choice = -1;
    slowprint("*Select a test from the following:\n\n");
    slowprint("*Test (1) : malloc() and immediately free() a 1-byte object, 120 times\n\n");
    slowprint("*Test (2) : Use malloc() to get 120 1-byte objects, storing the pointers in an array, then use free() to deallocate the chunks\n\n");
    slowprint("*Test (3) : Create an array of 120 pointers. Repeatedly make a random choice between allocating a 1-byte object and adding the pointer to the array and deallocating a previously allocated object (if any), until you have allocated 120 times. Deallocate any remaining objects\n\n");
    slowprint("*Test (4) : Generate one chunk with random payload size, malloc(), free() immediately, repeat 50 times.\n");
    slowprint("*Test (5) : Allocate until there is no more memory left. Then, randomly free the allocated pointers until all are freed.\n");
    slowprint("*Test (6) : Allocate 120 1-byte pointers, writing to the payload some data. Then, iterates over the pointers ensuring that the data contains the same data as stored. If an overwrite occurs, this method halts and notifies as such.\n");
    slowprint("*Test (7) : Allocates 120 pointers and ensures that each pointer is a unique pointer (no duplicates). This is a test to ensure malloc() returns unique pointers.\n");

    slowprint("*Exit (0) : Exit the program\n\n");
    slowprint("*Enter a number [0,6] :");
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