OBJS	= memgrind.o mymalloc.o
SOURCE	= memgrind.c mymalloc.c
HEADER	= 
OUT	= memgrind
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

memgrind.o: memgrind.c
	$(CC) $(FLAGS) memgrind.c -std=c99

mymalloc.o: mymalloc.c
	$(CC) $(FLAGS) mymalloc.c -std=c99


clean:
	rm -f $(OBJS) $(OUT)
