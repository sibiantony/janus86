AS 	= as
LD 	= ld
OBJS 	= \
	bootsect.bin \
	start.o \
	setup.o 
	
all: 	$(OBJS)

bootsect.bin: boot.s 
	$(AS) -o boot.o boot.s
	$(LD) -Ttext 0x7C00 --oformat binary -o bootsect.bin boot.o
start.o:
	as -o start.o start.s
setup.o:
	as -o setup.o setup.s
	
clean:
	rm -f *~
	rm -f *.o
	rm -f *.bin
