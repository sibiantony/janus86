
#ifndef __EXT2_H
#define __EXT2_H

#include "keyboard.h"
#include "types.h"
#include "video.h"
#include "kprintf.h"
#include "hd.h"
#include "kmalloc.h"
#include "superblock.h"
#include "dir.h"

#include "shell.h"

#define SECTOR_SIZE 512
/*superblock lies at a fixed offset of 1024 B from partition start;ie 2 sectors*/
#define SUPER_BLOCK_OFFSET 2

#define ROOT_INODE 0x02	

#define S_IFMT  00170000
#define S_IFSOCK 0140000
#define S_IFLNK	 0120000
#define S_IFREG  0100000
#define S_IFBLK  0060000
#define S_IFDIR  0040000
#define S_IFCHR  0020000
#define S_IFIFO  0010000
#define S_ISUID  0004000
#define S_ISGID  0002000
#define S_ISVTX  0001000

#define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)

struct ext2_inode {
	USHORT	i_mode;		/* File mode */
	USHORT	i_uid;		/* Owner Uid */
	UINT	i_size;		/* Size in bytes */
	UINT	i_atime;	/* Access time */
	UINT	i_ctime;	/* Creation time */
	UINT	i_mtime;	/* Modification time */
	UINT	i_dtime;	/* Deletion Time */
	USHORT	i_gid;		/* Group Id */
	USHORT	i_links_count;	/* Links count */
	UINT	i_blocks;	/* Blocks count */
	UINT	i_flags;	/* File flags */
	union {
		struct {
			UINT  l_i_reserved1;
		} linux1;
		struct {
			UINT  h_i_translator;
		} hurd1;
		struct {
			UINT  m_i_reserved1;
		} masix1;
	} osd1;				/* OS dependent 1 */
	UINT	i_block[15];/* Pointers to blocks */
	UINT	i_version;	/* File version (for NFS) */
	UINT	i_file_acl;	/* File ACL */
	UINT	i_dir_acl;	/* Directory ACL */
	UINT	i_faddr;	/* Fragment address */
	union {
		struct {
			UCHAR	l_i_frag;	/* Fragment number */
			UCHAR	l_i_fsize;	/* Fragment size */
			USHORT	i_pad1;
			UINT	l_i_reserved2[2];
		} linux2;
		struct {
			UCHAR	h_i_frag;	/* Fragment number */
			UCHAR	h_i_fsize;	/* Fragment size */
			USHORT	h_i_mode_high;
			USHORT	h_i_uid_high;
			USHORT	h_i_gid_high;
			UINT	h_i_author;
		} hurd2;
		struct {
			UCHAR	m_i_frag;	/* Fragment number */
			UCHAR	m_i_fsize;	/* Fragment size */
			USHORT	m_pad1;
			UINT	m_i_reserved2[2];
		} masix2;
	} osd2;				/* OS dependent 2 */
} *intab[4],*intab1[4];

struct group
{
	UINT block_bitmap;
	UINT inode_bitmap;
	UINT inode_table;
	USHORT free_blocks_count;
	USHORT free_inodes_count;
	USHORT used_dirs_count;
	USHORT pad;
	UINT reserved[3];
} *grblock;


struct ext2_dir_entry_2
{
	UINT inode;
	USHORT rec_len;
	UCHAR name_len;
	UCHAR file_type;
	char name[255];
} *direntry,*direntry1;

UINT inode_to_iba(UINT );
int get_index(UINT );
int ext2_rd_root (int , UINT );
int ext2_rd_inode( int, UINT, UINT );

#endif
