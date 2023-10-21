#include <stdio.h>

static int invocations;

#define MEMLENGTH 512

#define FREE 0
#define USED 1


double static memory [MEMLENGTH];

#define TRUE 1
#define FALSE 0

#define TESTING FALSE


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
    data.size = (MEMLENGTH * sizeof(double) - sizeof(meta_data));
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
        if(data -> usage == FREE && (data -> size >= size + sizeof(meta_data))) {
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
    if(TESTING) {
        printf("Malloc found an available pointer at address %p\n", data);
    }
    if(data -> next == NULL) {
        //we found the final chunk
        meta_data new_data;
        new_data.usage = FREE;
        new_data.next = NULL;
        
        char * data_address = ((char *)(data) + sizeof(meta_data) + size); //address we will put the new struct at in the memory
        new_data.size = (char *)(EOM) - (data_address + sizeof(meta_data));
        if(TESTING) {
            printf("Found next meta data address at address %p\n", data_address);
            printf("Size is %d\n", new_data.size);
        }
        *( (meta_data *) data_address) = new_data;
        data -> next = (meta_data *)(data_address);

    }
    data -> usage = USED;
    data -> size = size;
    return ((char *)(data) + sizeof(meta_data));
    }
    return NULL;
}

meta_data * find_left(meta_data * p) {
    meta_data * current = SOM;
    while(current != p) {
        if(current -> usage == FREE && current -> next == p) {
            return current;
        }
        current = current -> next;
    }
    return NULL;
}
meta_data * find_right(meta_data * p) {
    meta_data * next = p -> next;
    if(next != NULL && next -> usage == FREE) {
        return next;
    }
    return NULL;
}
/**
 * Joins the given META POINTER to adjcently free chunks and returns the META DATA address of the joined chunks.
*/
meta_data * join(meta_data * p) {
    meta_data * right = find_right(p);
    meta_data * left = find_left(p);

    if(right == NULL && left != NULL) {
        left -> size += (p -> size) + (sizeof(meta_data));
        left -> next = p -> next;
        return left;
    }
    if(right != NULL && left == NULL) {
        p -> size += (right -> size) + (sizeof(meta_data));
        p -> next = right -> next;
        return p;
    }
    if(right != NULL && left != NULL) {
        left -> size += (right->size) + (p->size) + (2*sizeof(meta_data));
        left -> next = right -> next;
        return left;
    }
    return p; //no left/right adjacently free

}
void myfree(void *p, char *file, int line) {
    meta_data * current = SOM;
    while(current != NULL) {
        if(TESTING) {
            printf("On meta chunk %p\n", current);
            printf("Checking if %p is the pointer\n", (void *)(current) + sizeof(meta_data));
        }
        void * check = ((void *)(current) + sizeof(meta_data));
        if(check == p) { //we found our pointer!
            if(current -> usage == FREE) {
                printf("free() failed in file '%s' on line '%d' as the pointer %p has already been freed\n", file, line, p);
            }
            else {
                current = join(current); //coelsce p
                current -> usage = FREE;
            }
            return;
        }
        current = current -> next;
    }
    printf("free() failed in file '%s' on line '%d' as the pointer %p does not point to a piece of allocated memory\n", file, line, p);
}

void print_mem() {
    meta_data * data = SOM;
    printf("-------------------------\n");
    printf("SOM : %p\n", SOM);
    printf("EOM : %p\n", EOM);
    while(data != NULL) {
        printf("On address %p\n", data);
        printf("Usage : %d\n", data -> usage);
        printf("Payload Size  : %d\n", (int)(data -> size));
        printf("Chunk Size : %lu\n\n", (int)(data -> size) + (sizeof(meta_data)));
        data = data -> next;
    }
    printf("-------------------------\n");
}