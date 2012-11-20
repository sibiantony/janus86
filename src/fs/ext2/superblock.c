
/* This file contains the ext2fs superblock functions. */
#include <superblock.h>

/*void printerror ( int errcode )
{
	if (errcode == FIL_NOT_FOUND) 	kprintf("File not Found\n");
	if (errcode == FIL_NOT_FOUND) 	kprintf("Not a Regular File\n");
	if (errcode == FIL_NOT_FOUND) 	kprintf("Directory not found\n");
	if (errcode == FIL_NOT_FOUND) 	kprintf("Disk Read Error\n");
	if (errcode == FIL_NOT_FOUND) 	kprintf("Path Length Exceeded\n");
	if (errcode == CD_ERR) 		kprintf("Cannot cd to a file\n");	
	if (errcode == ARG_LIST_EXCEED) kprintf("Argument List Exceeded\n");
}*/

int ext2_rd_superblock(int drive_num, UINT lba)
{
	UCHAR *super_buf;
	UCHAR msg[40];
	
	super_buf = (struct super_block *) kmalloc(sizeof(struct super_block));
	if( read_disk_sector(super_buf, drive_num, lba + SUPER_BLOCK_OFFSET, 2) == IDE_ERR_IO_ERROR )
		shell_panic(DISK_READ_ERR);
	ext2super = (struct super_block *)super_buf;
	if (ext2super->magic != EXT2_MAGIC) {
		/* not an ext2/3 partition	 */
		shell_panic(BAD_SUPER);	
		return FAILURE;
	}	
	gbinfo->bs = 1024 << ext2super->log_block_size;
	if ( gbinfo->bs == 1024 )
		gbinfo->gdoffset = 2;
	else
	{
		if ( gbinfo->bs == 4096 )
			gbinfo->gdoffset = 6;
		else
			gbinfo->gdoffset = 2;
	}

	return SUCCESS;
}


int ext2_rd_block(int drive_num, UINT lba, UINT sectors, UCHAR *buf)
{	
	if( read_disk_sector(buf, drive_num, ( lba ) + SUPER_BLOCK_OFFSET,
							 sectors) == IDE_ERR_IO_ERROR )
		shell_panic(DISK_READ_ERR);
}
