.code16					# we're still in real mode!

_start:
janus_start:
	
.globl rto_pmode;
rto_pmode:
.code16
	cli
	
	/* load global descriptor table  */
	xorw 	%ax, %ax
	movw	%ax, %ds              	# Set DS-register to 0 - used by lgdt
        DATA32	ADDR32 lgdt 	gdt_desc         	# Load the GDT descriptor


        movl 	%cr0, %eax            	# Copy the contents of CR0 into EAX
        orl 	$0x01, %eax             # Set bit 0
        movl 	%eax, %cr0            	# Copy the contents of EAX into CR0

        DATA32 ljmp 	$0x08, $pmode_seg	# Jump to code segment, offset clear_pipe

.code32					# We now need 32-bit instructions
pmode_seg:

	movw 	$0x10, %ax             	# Save data segment identifier
	movw 	%ax, %ds              	# Move a valid data segment into the ds reg.
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw 	%ax, %ss 		# Move a valid data segment into the ss reg.
	movl 	$0x090000, %esp        	# Move the stack pointer to 090000h

	call 	k_main
gdt:                    		# Address for the GDT
gdt_null:               		# Null Segment
        .word	0, 0
	.byte	0, 0, 0, 0

gdt_code:               		# Code segment, read/execute, nonconforming
	.word	0xFFFF, 0
	.byte	0, 0x9A, 0xCF, 0

gdt_data:               		# Data segment, read/write, expand down
        .word	0xFFFF, 0
	.byte	0, 0x92, 0xCF, 0

	/* 16 bit real mode CS */
	.word	0xFFFF, 0
	.byte	01, 0x9A, 0, 0

	/* 16 bit real mode DS */
	.word	0xFFFF, 0
	.byte	0, 0x92, 0, 0
	
gdt_end:                		# Used to calculate the size of the GDT
gdt_desc:                       	# The GDT descriptor
	.word gdt_end - gdt - 1    	# Limit (size)
	.long gdt                  	# Address of the GDT


