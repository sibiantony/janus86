
.code16				# we need 16 bit instructions in the real mode.
reset_drive:
        movb 	$0x0, %ah		# RESET-command
        int 	$0x13			# Call interrupt 13h
        orb 	%ah, %ah		# Check for error code
	jnz 	reset_drive		# Try again if ah != 0
	jmp 	load_kernel
	
load_sectors:
	pushw 	%ax
	pushw 	%bx
	pushw	%cx
	movb 	$0x12, %dl
	divb 	%dl
	inc 	%ah
	movb 	%ah, %cl		# sector number
	movb 	%al, %dh
	andb 	$0x01, %dh 		# head
	shrb 	$0x01, %al
	movb 	%al, %ch		# cylinder number
	movb 	$0x00, %dl 		# we are reading a floppy drive, dl=00h 
	
read:
	movb	$0x02, %ah		# Read sector command 
	movb 	$0x01, %al		# number of sectors - one sector at a time
	int 	$0x13
	jc 	read
	
	popw 	%cx
	popw	%bx
	popw 	%ax
	addw 	$0x0200, %bx		# increment the destination address by 512 bytes.
	inc 	%ax
	loop 	load_sectors
	ret

load_kernel:

	movw 	$0x1000, %ax		# segment addr. where we load our kernel.
	movw 	%ax, %es	
	movw 	$0x00, %bx		# offset value
	mov 	$0x01, %ax		# count for each sector read.
	mov 	$0x40, %cx 		# total number of sectors to read
	call	load_sectors
	
        cli			   	# Disable interrupts, we want to be alone

	/* Now enable the A20 gate. 	*/
	call	empty_8042
	movb	$0xD1,%al		# command write
	out	%al,$0x64
	call	empty_8042
	mov	$0xDF,%al		# A20 on
	out	%al,$0x60
	call	empty_8042	

	/* jump to the segment where we loaded the sectors */
	jmp	$0x1000, $0x0		
	
/* Checks whether the keyboard command queue is empty
*/
empty_8042:
	in	$0x64,%al	# 8042 status port
	test	$2,%al		# is input buffer full?
	jnz	empty_8042	# yes - loop
	ret

.org 0x1FE
	.word	 0xAA55		# Boot sector signature to identify as an x86 bootsector
