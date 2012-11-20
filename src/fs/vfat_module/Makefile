CC	= gcc
INCLUDE = include/
OBJS	= \
	fat32.o\
		
all : $(OBJS)

fat32.o:fat32.c
	$(CC) -c -I$(INCLUDE) -o fat32.o fat32.c
	ld -Ttext 0x20000 --oformat binary -o fat32.mo fat32.o \
			dep/diskio.o dep/hd.o 	\
			dep/string.o dep/kmalloc.o 	\
			dep/x86.o dep/keyboard.o 	

clean	:
	rm -rf fat32.o
	rm -rf *~	
