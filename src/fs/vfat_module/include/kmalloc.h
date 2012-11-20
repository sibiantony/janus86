#ifndef __KMALLOC_H
#define __KMALLOC_H

#include "mem.h"
#include "types.h"

typedef long Align; 			/* for alignment to long boundary */
 
/* Block header */
union header{				
	struct {
		union header *ptr;	/* next block if on free list */
		unsigned size;		/* size of this block	*/
	} s;
	Align x;			/* force alignment of blocks */
};

void *kmalloc(unsigned );
static union header *morecore(unsigned);
void kfree(void *);

#endif

