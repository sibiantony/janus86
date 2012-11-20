#include <kmalloc.h>

static union header *base ;			 /* empty list to get to started */
static union header *freep = NULL;/* start of free list */

/* kmalloc : storage allocator for the kernel */
void *kmalloc(unsigned nbytes)
{
	union header *p, *prevp;
	unsigned nunits ;

	nunits = (nbytes + sizeof(union header) - 1)/sizeof(union header) + 1;
	if ((prevp = freep) == NULL) 	/* no free list yet */
	{
		base = KERNEL_MEM_START;
		base->s.ptr = freep = prevp = base;
		base->s.size = KERNEL_MEM_SIZE/sizeof(union header);
	}
	for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr)
	{
		if (p->s.size >= nunits)		/* big enough */ 
		{
			if(p->s.size == nunits)		/* exactly */
				prevp->s.ptr = p->s.ptr;
			else				/* allocate tail end */
			{
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			return (void*)(p+1);
		}
		if (p == freep)				/* wrapped around free list */
			if ((p = morecore(nunits)) == NULL)
				return NULL;		/* none left */
	}
}


static union header *morecore(unsigned nu)
{
	return 0;
}

/* free : put block ap in free list */
void kfree(void *ap)
{
	union header *bp, *p;
	
	bp= (union header *) ap - 1;
	
	for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
	 	if(p >= p->s.ptr && (bp >p || bp < p->s.ptr))
			break;

 	if(bp + bp->s.size == p->s.ptr)
	{
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	}	
	else
		bp->s.ptr = p->s.ptr;
	if(p + p->s.size ==bp)
	{
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	}
	else
		p->s.ptr = bp;
	freep = p;
}


			

