NASM	= nasm -f elf
CC	= gcc -c 
INCLUDE = ../../include
OBJS	= \
	janus86.o \
	x86.o \
	bootinfo.o \
	kprintf.o \
	elf.o \
	delay.o \
	idt.o \
	string.o \
	sched.o \
	fs.o
	
all : $(OBJS)

kernel_start.o:kernel_start.asm
	$(NASM) -o kernel_start.o kernel_start.asm

janus86.o: janus86.c
	$(CC) -I$(INCLUDE) -o janus86.o janus86.c

x86.o	: x86.c
	$(CC) -I$(INCLUDE) -o x86.o x86.c

bootinfo.o	: bootinfo.c
	$(CC) -I$(INCLUDE) -o bootinfo.o bootinfo.c

kprintf.o	:kprintf.c
	$(CC) -I$(INCLUDE) -o kprintf.o kprintf.c

elf.o	:elf.c $(INCLUDE)/elf.h
	$(CC) -I$(INCLUDE) -o elf.o elf.c

delay.o	:delay.c $(INCLUDE)/delay.h
	$(CC) -I$(INCLUDE) -o delay.o delay.c

idt.o	:idt.c $(INCLUDE)/idt.h
	$(CC) -I$(INCLUDE) -o idt.o idt.c

sched.o	:sched.c $(INCLUDE)/sched.h
	$(CC) -I$(INCLUDE) -o sched.o sched.c

string.o:string.c $(INCLUDE)/string.h
	$(CC) -I$(INCLUDE) -o string.o string.c 

switch.o:switch.asm
	$(NASM) -o switch.o switch.asm
	
fs.o:fs.c $(INCLUDE)/fs.h
	$(CC) -I$(INCLUDE) -o fs.o fs.c 

clean	:
	rm -rf *.o
	rm -rf *~	
