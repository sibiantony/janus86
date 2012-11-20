#include <bootinfo.h>


void set_mem_info(multiboot_info_t *boot_info)
{
	mem_lower = boot_info->mem_lower;
	mem_upper = boot_info->mem_upper;

}
