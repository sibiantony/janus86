#include <delay.h>
extern void pit_delay_ISR(void);         /* Timer ISR for calibration   */

/*
 * The delay function:
 * Basically, delay_count is the number you have to count to in order to
 * kill 1ms.  delay() takes the amount of milliseconds to delay as
 * parameter.
 *
 * The goal of the rest of this code is to determine what delay_count *is*
 * for your machine.
 *
 * Keep delay_count initialised to 1.  We'll need that later.
 */

static unsigned long delay_count = 1;


void __delay(loops)
{
    	unsigned long c;
    	for(c=0;c<loops;c++);
}

void delay(unsigned long milliseconds)
{
    	__delay(milliseconds*delay_count);     /* Delay milliseconds ms */
}

#define MILLISEC 10           /* Take a 10ms fixed timer rate   */
#define FREQ (1000/MILLISEC)  /* Timer freqency for MILLISEC    */

static volatile unsigned long ticks = 0;

void delayCalibInt(void)      		/* The timer ISR                  */
{
    	ticks++;                  	/* Increment the ticks counter    */
}

#define PRECISION 8                 	/* Calibration precision          */

unsigned long calibrateDelayLoop(void)
{
    	unsigned int prevtick;          /* Temporary variable             */
    	unsigned int i;                 /* Counter variable               */
    	unsigned int calib_bit;         /* Bit to calibrate (see below)   */

    /* Initialise timer interrupt with MILLISECOND ms interval        */
    	init_pit(FREQ, 0);
    	set_vector(pit_delay_ISR, M_VEC, (D_INT + D_PRESENT + D_DPL3));
    	enable_irq(0);

    /* Stage 1:  Coarse calibration                                   */

    	do {
        	delay_count <<= 1;          /* Next delay count to try        */

        	prevtick=ticks;             /* Wait for the start of the next */
        	while(prevtick==ticks);     /* timer tick                     */

        	prevtick=ticks;             /* Start measurement now          */
        	__delay(delay_count);       /* Do the delay                   */
    	} while(prevtick == ticks);     	/* Until delay is just too big    */

    	delay_count >>= 1;              	/* Get bottom value for delay     */

    /* Stage 2:  Fine calibration                                     */

    	calib_bit = delay_count;        	/* Which bit are we going to test */

    	for(i=0;i<PRECISION;i++) {
        	calib_bit >>= 1;            /* Next bit to calibrate          */
        	if(!calib_bit) break;       /* If we have done all bits, stop */

        	delay_count |= calib_bit;   /* Set the bit in delay_count     */

        	prevtick=ticks;             /* Wait for the start of the next */
        	while(prevtick==ticks);     /* timer tick                     */

        	prevtick=ticks;             /* Start measurement now          */
        	__delay(delay_count);       /* Do the delay                   */

        	if(prevtick != ticks)       /* If a tick has passed, turn the */
           	 	delay_count &= ~calib_bit;     /* calibrated bit back off */
    	}

    /* We're finished:  Do the finishing touches                      */

    	disable_irq(0);                /* Our wonderful PIT can stop now  */

    	delay_count /= MILLISEC;       /* Calculate delay_count for 1ms   */
		kprintf("calibration",0,0);
		getch();

    return delay_count;
}


/*
 * The hardware part of the timer ISR:
 * This code is invoked when the PIT finishes its period.
 */

asm (
   ".globl pit_delay_ISR   \n"
   "pit_delay_ISR:         \n"
   "   pusha               \n"   /* Save all registers             */
   "   pushw %ds           \n"   /* Save user code data segment    */
   "   pushw %ss           \n"   /* Get kernel data segment        */
   "   popw %ds            \n"   /* %ss always valid (ss0 in TSS)  */
   "                       \n"
   "   call delayCalibInt  \n"   /* Call the actual ISR code       */
   "                       \n"
   "   movb $0x20,%al      \n"   /* Send EOI to the 8259           */
   "   outb %al,$0x20      \n"
   "   popw %ds            \n"   /* Restore user code data segment */
   "   popa                \n"   /* Restore registers              */
   "   iret                \n"   /* Return from interrupt          */
);
