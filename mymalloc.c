#include <stdio.h>

static int invocations;

#define MEMLENGTH 512

#define FREE 0
#define USED 1


double static memory [MEMLENGTH];

#define TRUE 1
#define FALSE 0


struct META_DATA { //16 byte size
    struct META_DATA * next; //8bytes
    int size;   //4 byte
    char usage; //1 byte
} typedef meta_data;


static meta_data * SOM = (meta_data *)memory;
static meta_data * EOM = (meta_data *)(memory + MEMLENGTH);

void init() {
    meta_data data;
    data.usage = FREE;  
    data.size = MEMLENGTH - sizeof(meta_data);
    data.next = NULL;
    *((meta_data *)(memory)) = data;
}

size_t round_up(size_t size) {
    return ((size+(7)) & ~(7));
}
int final_chunk(meta_data * data) {
    return data -> next == NULL;
}
meta_data * find(size_t size) {
    meta_data * data = (meta_data *)(memory);
    while(data != NULL) {
        if(data -> usage == FREE && data -> size >= size) {
            return data;
        }
        data = data -> next;
    }
    return data;
}
void *mymalloc(size_t size, char *file, int line) {
    if(invocations == 0) {
        init();
        invocations++;
    }
    if(size != 0) {
    size = round_up(size);
    meta_data * data = find(size);
    if(data == NULL) {
        printf("malloc() failed in file '%s' on line '%d'\n", file, line);
        return data;
    }
    printf("Malloc found an available pointer at address %p\n", data);
    if(data -> next == NULL) {
        //we found the final chunk
        meta_data new_data;
        new_data.usage = FREE;
        new_data.next = NULL;
        
        double * data_address = ((double *)(data) + sizeof(meta_data) + size); //address we will put the new struct at in the memory
        printf("Found next meta data address at address %p\n", data_address);
        new_data.size = (double *)(EOM) - (data_address + sizeof(meta_data));
        *( (meta_data *) data_address) = new_data;

        printf("Size is %d\n", new_data.size);
        data -> next = (meta_data *)(data_address);

    }
    data -> usage = USED;
    data -> size = size;
    return data;
    }
    return NULL;
}

void print_mem() {
    meta_data * data = SOM;
    printf("-------------------------\n");
    while(data != NULL) {
        printf("On address %p\n", data);
        printf("Usage : %d\n", data -> usage);
        printf("Size  : %d\n", (int)(data -> size));
        printf("Moving to address %p \n", data->next);
        data = data -> next;
    }
    printf("-------------------------\n");
}

