NASM	= nasm -f elf
CC	= gcc -c 
INCLUDE = ../../include
OBJS	= \
	shell.o \
	shell_builtin.o

all : $(OBJS)

shell.o:shell.c
	$(CC) -I$(INCLUDE) -o shell.o shell.c

shell_builtin.o:shell_builtin.c
	$(CC) -I$(INCLUDE) -o shell_builtin.o shell_builtin.c

clean	:
	rm -rf *.o
	rm -rf *~	

