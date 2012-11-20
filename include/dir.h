
#ifndef __DIR_H
#define __DIR_H

#include "keyboard.h"
#include "types.h"
#include "video.h"
#include "kprintf.h"
#include "hd.h"
#include "kmalloc.h"
#include "superblock.h"
#include "shell.h"
#include "ext2.h"


#define UNKNOWN 	0
#define REG_FILE 	1
#define DIRECTORY 	2
#define CHAR_DEV	3
#define BLK_DEV		4
#define PIPE		5
#define SOCKET		6	
#define SYM_LINK	7


struct sindirect
{
	UINT siblock;
}	*sind;

struct dindirect
{
	UINT diblock;
}	*dind;

struct file_properties
{
	UINT inode;
	UINT size;
	UINT file_type;
	UCHAR filename[255];
	struct file_properties *next;
}	*ext2_file_prop, *ext2_temp;
	
int init_ext2 ( int , UINT , UINT );
struct file_properties* ls_e ( int , UINT , int );
UCHAR * more_e (char *, int *, UCHAR* );
struct file_properties * chdir (char *);
void print_mode(UINT );
int ext2_rd_dir ( int , UINT , struct ext2_inode * );
int ext2_rd_file ( UINT ,struct ext2_inode *,UCHAR *);
int ext_load_file( UINT , UCHAR *);

#endif
