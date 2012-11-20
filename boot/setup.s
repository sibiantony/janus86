.code32
.globl pto_rmode;
	
/*Trying to get back to real mode ..
*/
pto_rmode:

	/* set up segment limits */
	movw	$0x20, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss

	/* jump to a 16 bit segment */
	ljmp	$0x18, $((tmpcseg) - 0x10000)	

tmpcseg:
	.code16

	/* clear the PE bit of CR0 with paging disabled */
	movl	%cr0, %eax
	andl 	$0x7ffffffe, %eax
	movl	%eax, %cr0

	/* flush prefetch queue, reload %cs */
	DATA32	ljmp	$0x1000, $((realcseg) - 0x10000)

realcseg:
	/* we are in real mode now
	 * set up the real mode segment registers : DS, SS, ES
	 */
	/* zero %eax */


	xorl	%eax, %eax

	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss

	/* restore interrupts */
	sti

	ljmp	$0x07C0, $0x0
