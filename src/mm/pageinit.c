#include <pageinit.h>

/*
*	This is where we initialize page directories and 
	page tables.
	The physical memory after paging is as shown,
	
			|_______________|
			| ...		|
			| Padded by 0's	|
			|_______________|
			| ...		|
			|_______________|
			|  0x00001003	|
			|_______________|
			|  0x00000003	|
(Page Table Entries) +4k|_______________|
			| ....		|
			 Padded by 0's
			|_______________|
			  ....
			|_______________|
			| 0x00001000	|
			|_______________|
			| 0x00000000 	|
	PDB(CR3) -->	|_______________|

*/


void page_init()
{
	unsigned int pg_tables,pg_frames;
	unsigned int pg_table_entry;
	unsigned int pg_dir_entry;
	unsigned int pg_offset;
	unsigned int *kptr;
	unsigned int i;

	pg_frames = MEM_SIZE >> 12;
	i = pg_tables = pg_frames >> 10 + 1;
	
	/*Initialize the page dir for write-through/supervisor/cached/4k/local pages*/
	pg_offset = 0x00001000;
	
	
	pg_dir_entry = pg_offset | PAGE_PRESENT |
				 PAGE_READ_WRITE |
				 PAGE_TABLE_ADDR;

	for(kptr = (int *)PAGE_TABLE_ADDR; i--; pg_dir_entry += pg_offset){
		*( kptr++) = pg_dir_entry;
	}
	for(;(unsigned long)kptr & 0xfff;){
		*(kptr++) = 0;
	}

	/*Fill the page tables*/
	pg_table_entry = PAGE_PRESENT | PAGE_READ_WRITE ;
	for(i = pg_frames; i--; pg_table_entry += pg_offset){
		*(kptr++) = pg_table_entry;
	}
	for(;(unsigned long)kptr & 0xfff;){
		*(kptr++) = 0;
	}
}


void mem_init()
{
	unsigned int ret;
	unsigned int pg_table_address;
	page_init();

	pg_table_address = PAGE_TABLE_ADDR ;

	/*Initialize the mm allocator*/

	
	/*Start paging*/
	__asm__ __volatile__(
			"movl %0,%%eax;"
			"movl %%eax,%%cr3;"
			"movl %%cr0,%%eax;"
			"orl $0x80000000,%%eax;"
			"movl %%eax,%%cr0;"	/*This is serializing, so jump isn't needed*/
			:
			:"g"(pg_table_address) 
			:"eax"
			);
}
