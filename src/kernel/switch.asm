 [BITS 32]

 SECTION .text

 GLOBAL task_timer
 EXTERN task_timer_c

 task_timer:
	pushad
    	cli
    	push ds
    	push es
    	push fs
    	push gs
    	mov ax, 0x00
    	mov ds, ax
    	mov es, ax
    	mov fs, ax
    	mov gs, ax
    	push esp
    	mov eax, task_timer_c
    	call eax
    	pop esp
    	mov esp, eax
    	pop gs
    	pop fs
    	pop es
    	pop ds
    	sti
    	popad
    	ret
