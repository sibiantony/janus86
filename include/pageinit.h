#ifndef __PAGEINIT_H
#define __PAGEINTI_H

#define MEM_SIZE 		134217728
#define PAGE_TABLE_ADDR		0x01000000	/* Location where Page table is loaded 	*/

/* Page directory entry attribute bits 	*/

#define PAGE_PRESENT		0x0001
#define PAGE_READ_WRITE		0x0002
#define PAGE_SUPERVISOR		0x0004
#define PAGE_WRITE_THROUGH	0x0008
#define PAGE_CACHE_DISABLED	0x0010
#define PAGE_ACCESSED		0x0020
#define PAGE_DIRTY		0x0040
#define PAGE_SIZE		0x0080
#define PAGE_ATTRIBUTE_INDEX	0x0080
#define PAGE_GLOBAL		0x0100

void page_init(void);
void mem_init(void);

#endif
