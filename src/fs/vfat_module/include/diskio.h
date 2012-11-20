/*
 *	Contains disk read/write functions .Also scans for 
 *	available partitions in the hard disk	.
 *
 */
#ifndef __DISKIO_H
#define __DISKIO_H

#include "keyboard.h"
#include "types.h"
#include "video.h"
#include "kprintf.h"
#include "hd.h"
#include "kmalloc.h"


/* 16 byte partition entry starting from byte 446 of MBR
 * There are 4 such 16 byte partition entries
 */
struct partition_entry {

	UCHAR active;	
	UCHAR start_head;
	UCHAR ssc1;		/* starting sector+cylinder [CCSS SSSS ---- ----] */
	UCHAR ssc2;		/* starting sector+cylinder [---- ---- CCCC CCCC] */	
	UCHAR type;		/* partition type byte */
	UCHAR end_head;	
	UCHAR esc1;		/* ending sector+cylinder [CCSS SSSS ---- ----] */
	UCHAR esc2;		/* ending sector+cylinder [---- ---- CCCC CCCC] */
	ULONG start_lba;	/* starting LBA value */
	ULONG num_sec;		/* size of partition in sectors */
} ;

/* The bootsector structure	*/
struct bootsector
{   
	UCHAR temp446[446];		/* skip the first 446 bytes */
	struct partition_entry part[4];
	USHORT signature;		/* the magic signature AA55 */
	struct bootsector *next;
}*bslist,*temp;

/******** struct to store the details of the partitions     ****/
struct part_props
{
	int name;	// ******* change *******/
	unsigned char type_byte;
	char type[10];
	UINT st_lba;
	struct part_props *next;
}	*partition, *part_temp;

int part_in;


/* The MBR signature	*/
#define MBR_SIGNATURE 		0xAA55

/* Various partition types */
#define PART_UNKNOWN		0x00		
#define PART_DOS_FAT12 		0x01	/* 12-bit FAT. */  
#define PART_DOS3_FAT16		0x04	/* 16-bit FAT. Partition smaller than 32MB.	*/
#define PART_DOS_EXTENDED 	0x05	/* Extended MS-DOS Partition.			*/
#define PART_DOS4_FAT16		0x06	/* 16-bit FAT. Partition >= 32MB.		*/
#define PART_WIN_NTFS		0x07	
#define PART_WIN95_FAT32 	0x0B	/* 32-bit FAT. Partition up to 2047GB.		*/
#define PART_WIN95_FAT32_LBA 	0x0C	/* Same as FAT32(0Bh), but uses LBA		*/
#define PART_DOS_FAT16_LBA	0x0E	/* Same as PART_DOS4_FAT16(06h), but uses LBA  	*/
#define PART_WIN95_EXTENDED	0x0F	/* Same as PART_EXTENDED(05h), but uses LBA  	*/
#define PART_LINUX_SWAP		0x82	/* Linux Swap	*/
#define PART_LINUX_NATIVE	0x83	/* Linux Native , usually ext2fs */
#define PART_LINUX_EXTENDED	0x85	
#define PART_LINUX_HIDDEN1	0x93	
#define PART_BSD_386		0xA5	/* BSD/386, 386BSD, NetBSD, FreeBSD		*/
#define PART_OPENBSD		0xA6
#define PART_NETBSD		0xA9
#define PART_LINUX_HIDDEN2	0xC2


int numlin, numfat32;
//int peslba[MAX_LINUX_PART];
//int fat32lba[MAX_FAT32_PART];

void read_disk_partitions ();
int read_disk_sector (UCHAR *, int, UINT, UINT );
int write_disk_sector (UCHAR *, int, UINT, UINT );

#endif
