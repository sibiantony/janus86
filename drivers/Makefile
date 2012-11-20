CC	= gcc -c 
INCLUDE = ../include/
OBJS	= \
	video.o \
	keyboard.o \
	hd.o \
	diskio.o \
	8259.o \
	pit.o 	\
	serial.o

all 	:$(OBJS)

video.o	:video.c
	$(CC) -I$(INCLUDE) -o video.o video.c

keyboard.o:keyboard.c
	$(CC) -I$(INCLUDE) -o keyboard.o keyboard.c 

hd.o	:hd.c 
	$(CC) -I$(INCLUDE) -o hd.o hd.c 
	
diskio.o:diskio.c 
	$(CC) -I$(INCLUDE) -o diskio.o diskio.c 

8259.o	:8259.c
	$(CC) -I$(INCLUDE) -o 8259.o 8259.c

pit.o	:pit.c
	$(CC) -I$(INCLUDE) -o pit.o pit.c

serial.o:serial.c
	$(CC) -I$(INCLUDE) -o serial.o serial.c

clean	:
	rm -rf *.o
	rm -rf *~	
