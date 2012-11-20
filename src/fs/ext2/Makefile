CC	= gcc -c 
INCLUDE = ../../../include
OBJS	= \
	superblock.o\
	ext2.o\
	dir.o
	
all : $(OBJS)

superblock.o:superblock.c
	$(CC) -I$(INCLUDE) -o superblock.o superblock.c
ext2.o:ext2.c
	$(CC) -I$(INCLUDE) -o ext2.o ext2.c
dir.o:dir.c
	$(CC) -I$(INCLUDE) -o dir.o dir.c

clean	:
	rm -rf *.o
	rm -rf *~	
