
#ifndef __FAT32_H
#define __FAT32_H

#include "keyboard.h"
#include "types.h"
#include "video.h"

#include "hd.h"
#include "diskio.h"	
#include "kmalloc.h"


/*	File types	*/
#define UNKNOWN 	0
#define REG_FILE 	1
#define DIRECTORY 	2
#define CHAR_DEV	3
#define BLK_DEV		4
#define PIPE		5
#define SOCKET		6	
#define SYM_LINK	7



#define DIRENT_PER_SECTOR	16
#define LONG_NAME		1
#define SHORT_NAME		0
#define CLUSTER_TO_LBA(e)  (((e)-2)*sectors_per_cluster + cluster_begin_lba)

#define FAT_ATTR_READ_ONLY	0x01
#define FAT_ATTR_HIDDEN		0x02
#define FAT_ATTR_SYSTEM		0x04
#define FAT_ATTR_VOLUME_ID	0x08
#define FAT_ATTR_DIRECTORY	0x10
#define FAT_ATTR_ARCHIVE	0x20
#define FAT_ATTR_FILE		(FAT_ATTR_READ_ONLY|FAT_ATTR_HIDDEN|FAT_ATTR_SYSTEM|FAT_ATTR_ARCHIVE)
#define FAT_ATTR_LONG_NAME    	(FAT_ATTR_READ_ONLY|FAT_ATTR_HIDDEN|FAT_ATTR_SYSTEM|FAT_ATTR_VOLUME_ID)

#define FAT_EOF_MARKER		0x0FFFFFFF
#define CLU_PER_FAT		0x80

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

struct fat_file_properties
{
	UINT st_clust;
	UINT size;
	UINT file_type;
	UCHAR filename[255];
	struct fat_file_properties *next;
}	*fat_file_prop, *fat_temp;

struct fat_bootsector		/* Warning: this struct must be packed */
{
	UCHAR jump[3]	__attribute__ ((packed));/* 16-bit JMP to boot code, or 8-bit JMP + NOP */
	UCHAR oem_id[8]	__attribute__ ((packed));		/* e.g. 'MSWIN4.0' */
	USHORT bytes_per_sector	__attribute__ ((packed));	/* usu. =512 */
	UCHAR sectors_per_cluster	__attribute__ ((packed));
	USHORT num_boot_sectors __attribute__ ((packed));	/* usu. =1 */
	UCHAR num_fats	__attribute__ ((packed));		/* usu. =2 */
	USHORT num_root_dir_ents	__attribute__ ((packed));
	USHORT total_sectors	__attribute__ ((packed));	/* 16-bit; 0 if num sectors > 65535 */
	UCHAR media_ID_byte	__attribute__ ((packed));       /* usu. =0F0h */
	USHORT sectors_per_fat	__attribute__ ((packed));
	USHORT sectors_per_track	__attribute__ ((packed));
	USHORT heads	__attribute__ ((packed));
	UINT hidden_sectors	__attribute__ ((packed));	/* =LBA partition start */
	UINT total_sectors_large	__attribute__ ((packed));/* 32-bit; 0 if num sectors < 65536 */
	UINT big_sectors_per_FAT	__attribute__ ((packed));
 	USHORT ext_flags	__attribute__ ((packed)); /// uint to ushort.........<--
	USHORT  FS_Version	__attribute__ ((packed));
	UINT root_directory_start	__attribute__ ((packed));
	USHORT FS_info_sector	__attribute__ ((packed));
 	USHORT backup_boot_sector	__attribute__ ((packed));
	UCHAR reserved[12]	__attribute__ ((packed));
	UCHAR log_drv_num	__attribute__ ((packed));
	UCHAR unused	__attribute__ ((packed));	
	UCHAR ext_signature	__attribute__ ((packed));
	UINT serial_num	__attribute__ ((packed));
	UCHAR vol_nam[11]	__attribute__ ((packed));
	UCHAR FAT_nam[8]	__attribute__ ((packed));
	UCHAR boot_code[420]	__attribute__ ((packed));
	USHORT magic	__attribute__ ((packed));		/* 55h, 0AAh */
} *fat32boot;                              /* 512 bytes total */

struct fat_dirent               /* Warning: this struct must be packed */
{
        UCHAR name[8]	__attribute__ ((packed));          /* ALL-CAPS, pad right with spaces */
        UCHAR ext[3]	__attribute__ ((packed));          /* ALL-CAPS, pad right with spaces */
        UCHAR attrib	__attribute__ ((packed));          /* attribute byte */
        UCHAR reserved	__attribute__ ((packed));          /* =0 */
        UCHAR ctime_ms	__attribute__ ((packed));          /* file creation time, 10ms units */
        USHORT ctime	__attribute__ ((packed));          /* file creation time, in DOS format */
        USHORT cdate	__attribute__ ((packed));          /* file creation date, in DOS format */
        USHORT adate	__attribute__ ((packed));          /* DOS date of last file access */
        USHORT st_clust_msw	__attribute__ ((packed));  /* high 16 bits of starting cluster */
        USHORT mtime	__attribute__ ((packed));          /* DOS time of last file modification */
        USHORT mdate	__attribute__ ((packed));          /* DOS date of last file modification */
        USHORT st_clust	__attribute__ ((packed));          /* starting cluster */
        UINT file_size	__attribute__ ((packed));          /* in bytes */
}	*fat32dir;                              /* 32 bytes total */

struct fat_long_dirent
{
	UCHAR	LDIR_Ord	__attribute__ ((packed));
	UCHAR	LDIR_Name1[10]	__attribute__ ((packed));
	UCHAR	LDIR_Attr	__attribute__ ((packed));	
	UCHAR	LDIR_Type	__attribute__ ((packed));
	UCHAR	LDIR_Chksum	__attribute__ ((packed));
	UCHAR	LDIR_Name2[12]	__attribute__ ((packed));
	USHORT	LDIR_FstClusLO	__attribute__ ((packed));
	UCHAR	LDIR_Name3[4]	__attribute__ ((packed));
}	*fat32longdir;

struct dos_time                 /* Warning: this struct must be packed */
{
        unsigned two_secs : 5	__attribute__ ((packed));  /* low 5 bits: 2-second increments */
        unsigned minutes : 6	__attribute__ ((packed));   /* middle 6 bits: minutes */
        unsigned hours : 5	__attribute__ ((packed));     /* high 5 bits: hours (0-23) */
}	*d_time;                              /* 2 bytes total */

struct dos_date                 /* Warning: this struct must be packed */
{
        unsigned date : 5	__attribute__ ((packed));      /* low 5 bits: date (1-31) */
        unsigned month : 4	__attribute__ ((packed));     /* middle 4 bits: month (1-12) */
        unsigned year : 7	__attribute__ ((packed));      /* high 7 bits: year - 1980 */
}	*d_date;                              /* 2 bytes total */

struct FAT_32bit_entry
{
	UINT fat32bit[128];
}	*FAT;

	
UINT 	fat_begin_lba;
UINT 	cluster_begin_lba; 
UCHAR 	sectors_per_cluster;
UINT	root_dir_first_cluster;
UINT	RootDirSectors;
UINT	FirstDataSector;

//int item_count=0;			//just to maintain a count, to be deleted

void Check_char( char,int  );
int Generate_Long_Name (int , UINT, UINT ,UCHAR [] , int, int );
struct fat_file_properties * ls_fat32 ( int , UINT , int );
UCHAR * more_fat32 ( char *, int *, UCHAR*);
int init_fat32 (int ,  UINT , UINT );
struct fat_file_properties* chdir_fat32( char *);
int fat32_rd_FAT(int , UINT , UINT );
int fat32_rd_bootsec(int , UINT );
int fat32_rd_cluster ( int , UINT, UINT );
int fat32_mmap( int , UINT , UINT ,UCHAR *, int );


#endif

