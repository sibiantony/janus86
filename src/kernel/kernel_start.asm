MULTIBOOT_PAGE_ALIGN 		equ 	1<<0
MULTIBOOT_MEMORY_INFO 		equ 	1<<1
MULTIBOOT_AOUT_KLUDGE 		equ 	1<<16
MULTIBOOT_HEADER_MAGIC		equ 	0x1BADB002
MULTIBOOT_HEADER_FLAGS 		equ 	MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO |MULTIBOOT_AOUT_KLUDGE
CHECKSUM 			equ 	-(MULTIBOOT_HEADER_MAGIC +MULTIBOOT_HEADER_FLAGS)
	
[BITS 32]
[global start]
[extern k_main]
mboot:
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd CHECKSUM
	dd mboot
	dd start	
start:	
	push ebx	; push the pointer in ebx onto the stack
			; for accessing the grub multiboot info
	call k_main
	
	hlt
