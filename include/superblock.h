/*  
*	Header file to read and verify the superblock
*	of the ext2/3 filesystem
*
*/

#ifndef __SUPERBLOCK_H
#define __SUPERBLOCK_H

#include "keyboard.h"
#include "types.h"
#include "video.h"
#include "kprintf.h"
#include "hd.h"
#include "kmalloc.h"

/*	Definition of the Errors	*/
#define MOD_NOT_LOAD		-1
#define FIL_NOT_FOUND		-5
#define NOT_REG_FILE		-6
#define DIR_NOT_FOUND		-7
#define DISK_READ_ERR		-8
#define PAT_LEN_EXCEED		-9
#define CD_ERR			-10
#define BAD_FAT_BOOT_SECTOR	-11
#define BAD_SUPER		-12
#define ARG_LIST_EXCEED		-13
#define PART_NO_SUPPORT		-14
#define NO_SUCH_PART		-15
#define BAD_SIG			-16

#define SUPER_BLOCK_OFFSET 2	/* Super Block offset = 2 sectors */
#define EXT2_MAGIC 0xEF53	/* Ext2/3 magic word */

void printerror (int);
int ext2_rd_superblock (int, UINT);
int ext2_rd_block(int , UINT , UINT, UCHAR *);

int ext2_bs;	//block size

struct super_block /*structure of super block*/
{
	UINT	inodes_count;		/* Inodes count */
	UINT	blocks_count;		/* Blocks count */
	UINT	r_blocks_count;	/* Reserved blocks count */
	UINT	free_blocks_count;	/* Free blocks count */
	UINT	free_inodes_count;	/* Free inodes count*/
	UINT	first_data_block;	/* First Data Block */
	UINT	log_block_size;	/* Block size */
      	UINT	log_frag_size;	/* Fragment size *//////////////
	UINT	blocks_per_group;	/* # Blocks per group */
	UINT	frags_per_group;	/* # Fragments per group */
	UINT	inodes_per_group;	/* # Inodes per group */
	UINT	mtime;		/* Mount time */
	UINT	wtime;		/* Write time */
	USHORT	mnt_count;		/* Mount count */
	USHORT   max_mnt_count;	/* Maximal mount count */
	USHORT	magic;		/* Magic signature */
	USHORT	state;		/* File system state */
	USHORT	errors;		/* Behaviour when detecting errors */
	USHORT	minor_rev_level; 	/* minor revision level */
	UINT	lastcheck;		/* time of last check */
	UINT	checkinterval;	/* max. time between checks */
	UINT	creator_os;		/* OS */
	UINT	rev_level;		/* Revision level */
	USHORT	def_resuid;		/* Default uid for reserved blocks */
	USHORT	def_resgid;		/* Default gid for reserved blocks */
	UINT   first_ino;
	USHORT 	inode_size;
	USHORT   block_group_nr;
	UINT   feature_compat;
	UINT   feature_incompat;
	UINT   feature_ro_compat;
	UCHAR    uuid[16];
	char    volume_name[16];
	char    last_mounted[64];
	UINT   algorithm_usage_bitmap;
	UCHAR    prealloc_blocks;
	UCHAR    prealloc_dir_blocks;
	UCHAR    padding[818];	
} *ext2super;

struct global_info
{
	UINT bs;
	UINT gdoffset;
}	*gbinfo;

#endif
