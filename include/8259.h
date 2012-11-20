
#ifndef __8259_H
#define __8259_H

void Init8259();
void enable_irq(unsigned short irq_num);
void disable_irq(unsigned short irq_num);

#define M_PIC  0x20     /* I/O for master PIC              */
#define M_IMR  0x21     /* I/O for master IMR              */
#define S_PIC  0xA0     /* I/O for slave PIC               */
#define S_IMR  0xA1     /* I/O for slace IMR               */

#define EOI    0x20     /* EOI command                     */

#define ICW1   0x11     /* Cascade, Edge triggered         */
                        /* ICW2 is vector                  */
                        /* ICW3 is slave bitmap or number  */
#define ICW4   0x01     /* 8088 mode                       */

#define M_VEC  0x68     /* Vector for master               */
#define S_VEC  0x70     /* Vector for slave                */

#define OCW3_IRR  0x0A  /* Read IRR                        */
#define OCW3_ISR  0x0B  /* Read ISR                        */

#endif
