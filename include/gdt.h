#ifndef __GDT_H
#define __GDT_H

/*
 *  Each descriptor should have exactly one of next 8 codes to define the
 *  type of descriptor
 */

#define D_LDT   0x200   /* LDT segment        */
#define D_TASK  0x500   /* Task gate          */
#define D_TSS   0x900   /* TSS                */
#define D_CALL  0x0C00  /* 386 call gate      */
#define D_INT   0x0E00  /* 386 interrupt gate */
#define D_TRAP  0x0F00  /* 386 trap gate      */
#define D_DATA  0x1000  /* Data segment       */
#define D_CODE  0x1800  /* Code segment       */


/*
 *  Descriptors may include the following as appropriate:
 */

#define D_DPL3         0x6000   /* DPL3 or mask for DPL                    */
#define D_DPL2         0x4000   /* DPL2 or mask for DPL                    */
#define D_DPL1         0x2000   /* DPL1 or mask for DPL                    */
#define D_PRESENT      0x8000   /* Present                                 */
#define D_NOT_PRESENT  0x8000   /* Not Present                             */
                                /* Note, the PRESENT bit is set by default */
                                /* Include NOT_PRESENT to turn it off      */
                                /* Do not specify D_PRESENT                */


/*
 *  Segment descriptors (not gates) may include:
 */

#define D_ACC      0x100  /* Accessed (Data or Code)          */

#define D_WRITE    0x200  /* Writable (Data segments only)    */
#define D_READ     0x200  /* Readable (Code segments only)    */
#define D_BUSY     0x200  /* Busy (TSS only)                  */

#define D_EXDOWN   0x400  /* Expand down (Data segments only) */
#define D_CONFORM  0x400  /* Conforming (Code segments only)  */

#define D_BIG      0x40	  /* Default to 32 bit mode           */
#define D_BIG_LIM  0x80	  /* Limit is in 4K units             */


/*
 *  Now we define the structures for descriptors and gates:
 */

struct x86_desc {
   	unsigned short limit_low;     /* limit 0..15    */
   	unsigned short base_low;      /* base  0..15    */
   	unsigned char base_med;       /* base  16..23   */
   	unsigned char access;         /* access byte    */
   	unsigned int limit_high:4;    /* limit 16..19   */
   	unsigned int granularity:4;   /* granularity    */
   	unsigned char base_high;      /* base 24..31    */
} __attribute__ ((packed));


struct x86_gate {
   	unsigned short offset_low;   /* offset 0..15    */
   	unsigned short selector;     /* selector        */	
   	unsigned short access;       /* access flags    */
   	unsigned short offset_high;  /* offset 16..31   */
} __attribute__ ((packed));


/*
 *  Descriptor tables are basically arrays of either standard or gate
 *  descriptors;  The exception is the first descriptor, which is a
 *  'dummy' and can be filled with anything without crashing the machine.
 *
 */

union DT_entry {
   	struct x86_desc desc;     /* Normal descriptor */
   	struct x86_gate gate;     /* Gate descriptor   */
   	unsigned long long dummy; /* Any other info    */
};


#define desc_table(name,length) union DT_entry name[length] = 
#define stnd_desc(base, limit, control) {desc: {(limit & 0xffff), (base & 0xffff), ((base >> 16) & 0xff), \
                                               ((control+D_PRESENT) >> 8), (limit >> 16), \
                                               ((control & 0xff) >> 4), (base >> 24)}}
#define gate_desc(offset, selector, control) {gate: {(offset & 0xffff), selector, \
                                                     (control+D_PRESENT), (offset >> 16) }}


#endif
