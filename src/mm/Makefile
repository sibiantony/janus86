CC	= gcc -c 
INCLUDE = ../../include
OBJS	= \
	pageinit.o\
	kmalloc.o
all : $(OBJS)

pageinit.o:pageinit.c
	$(CC) -I$(INCLUDE) -o pageinit.o pageinit.c
kmalloc.o :kmalloc.c
	$(CC) -I$(INCLUDE) -o kmalloc.o kmalloc.c
clean	:
	rm -rf *.o
	rm -rf *~	
