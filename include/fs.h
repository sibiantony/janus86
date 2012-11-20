#ifndef __FS_H
#define __FS_H


/* structure for a registered filesystem module*/
typedef struct fs_drv_struct {
	unsigned char fs_type_byte;
	unsigned long load_mem;
	char drv_name[10];
	unsigned long sym_root;
	unsigned long sym_chdir;
	unsigned long sym_loadfile;
	struct fs_drv_struct *drv_next;
}fs_drv_reg;

fs_drv_reg 	*fs_drv_list;

/* structure for a loaded fs module header*/
typedef struct fs_drv_mod_struct {
	unsigned short mod_sign;
	unsigned short mod_size;
	unsigned long load_mem;
	unsigned long start_addr;
	unsigned long sym_tab_addr;
	unsigned char fs_type_byte;
	char mod_name[10];		
}fs_drv_mod;

/* The symbol table format in the module */
typedef struct fs_sym_tab_struct {
	char sym_name[10];
	unsigned long sym_val;
}fs_sym_tab;

struct fs_properties {	
	unsigned int start;
	unsigned int size;
	unsigned int file_type;
	unsigned char filename[255];
	struct fs_properties *next;
}	*fs_prop, *fs_temp, *fs_more;

#define EXT2_COMPILED_STATIC

#define EXT2_DRV_NAME 		"ext2"
#define FAT32_DRV_NAME		"vfat"

#define FS_DRV_NOT_FOUND	0
#define FS_DRV_FOUND		1
#define FS_MODULE_REGISTERED	2
#define FS_BAD_MODULE_FMT	-1
#define FS_MODULE_REMOVED	1
#define FS_MODULE_NOT_FOUND	0

/* Number of exported functions in a fs module . At present 3*/
#define FS_EXPORTED_NUM 	3

#include "diskio.h"
#include "kmalloc.h"
#include "string.h"
#include "kprintf.h"

int init_fs_modules ();
int fs_root (int);
int register_module (unsigned char*);
int unregister_module (char *);
struct fs_properties* fs_chdir (char *);
int fs_loadfile (char *, unsigned char *);
void list_drivers ();

#endif
