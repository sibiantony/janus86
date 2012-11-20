/*
 *	Contains disk read/write functions .Also scans for 
 *	available partitions in the hard disk	.
 *
 */

#include <diskio.h>


/*
*	Reads the raw disk sectors
*/
int read_disk_sector (UCHAR *buf, int drive_num, UINT lba, UINT num_sectors )
{
	return (read_sector (buf, drive_num, lba, num_sectors));
}
	
/*
 *	Write the raw disk sectors
 */	
int write_disk_sector (UCHAR *buf, int drive_num, UINT lba, UINT num_sectors )
{	
	return (write_sector (buf, drive_num, lba, num_sectors));
}

/* 
 * Read the MBR.The first 446 bytes are skipped.The next 64 bytes contains 
 * the partition table, and the last 2 bytes is the MBR signature "AA55"
 */
int read_mbr (int drive_num)
{
	int i;
	UCHAR buf[512];
	
	bslist = (struct bootsector *)kmalloc (sizeof (struct bootsector));
	bslist->next = NULL;
	
	if (read_disk_sector(buf, drive_num, 0, 1) == IDE_ERR_IO_ERROR)
//		kprintf("\nMBR Read error");
		return DISK_READ_ERR;
	for (i = 0; i < 4; i ++)
		bslist->part[i] = *((struct partition_entry *) &buf[446 + i*16]);
    	
	bslist->signature = *((USHORT *)&buf[510]);
}

/*	This function fills in the proprties of each partition in a linked list	*/
void save_part_props (struct partition_entry *pe, ULONG lba )
{
	part_temp->name = part_in;
		
	switch (pe->type)
	{
		case PART_UNKNOWN:	
			strcpy(part_temp->type, "Unknown");
			break;
		case PART_DOS_FAT12:
			strcpy(part_temp->type, "DOS_FAT12");
			break;
		case PART_DOS3_FAT16:	
			strcpy(part_temp->type, "DOS3_FAT16");
			break;
		case PART_DOS_EXTENDED:	
			strcpy(part_temp->type, "DOS3_Extended");
			break;
		case PART_DOS4_FAT16:	
			strcpy(part_temp->type, "DOS4_FAT16");
			break;
		case PART_WIN_NTFS:	
			strcpy(part_temp->type, "NTFS");
			break;
		case PART_WIN95_FAT32:	
			strcpy(part_temp->type, "FAT32");
			break;
		case PART_WIN95_FAT32_LBA:	
			strcpy(part_temp->type, "FAT32");
			break;
		case PART_DOS_FAT16_LBA:	
			strcpy(part_temp->type, "FAT16");
			break;
		case PART_WIN95_EXTENDED:	
			strcpy(part_temp->type, "Win95_Ext");
			break;
		case PART_LINUX_SWAP:	
			strcpy(part_temp->type, "Lin Swap");
			break;
		case PART_LINUX_NATIVE:	
			strcpy(part_temp->type, "LINUX");
			break;
		case PART_LINUX_EXTENDED:
			strcpy(part_temp->type, "Linux Ext");
			break;
		case PART_LINUX_HIDDEN1:	
			strcpy(part_temp->type, "LINUX");
			break;
		case PART_BSD_386:	
			strcpy(part_temp->type, "BSD_386");
			break;
		case PART_OPENBSD:	
			strcpy(part_temp->type, "OpenBSD");
			break;
		case PART_NETBSD:	
			strcpy(part_temp->type, "NetBSD");
			break;
		case PART_LINUX_HIDDEN2:	
			strcpy(part_temp->type, "LINUX");
			break;
	}
	if (pe->type == PART_LINUX_HIDDEN1 || pe->type == PART_LINUX_NATIVE) {
		current_lba = lba + pe->start_lba;
		part_temp->st_lba = lba + pe->start_lba;
		part_temp->type_byte = pe->type;
	}
	else if (pe->type == PART_WIN95_FAT32_LBA || pe->type == PART_WIN95_FAT32) {
		part_temp->st_lba = lba + pe->start_lba;
		part_temp->type_byte = pe->type;
	}
	else part_temp->st_lba = lba + pe->start_lba;
		
	part_temp->part_size = pe->num_sec  / 2048;
	
	part_in++;
	part_temp->next = (struct part_props *)kmalloc(sizeof(struct part_props));	
	part_temp = part_temp->next;
	part_temp->next = NULL;
}
/*
 * Traverse the 4 primary partitions.
 */
int traverse_primary_partitions ()
{
	if (bslist->signature == MBR_SIGNATURE)
	{   
		save_part_props (&bslist->part[0], 0);
		save_part_props (&bslist->part[1], 0);
		save_part_props (&bslist->part[2], 0);
		save_part_props (&bslist->part[3], 0);
	}
	else return BAD_SIG;
//		kprintf("\ntraverse_primary_partitions : Bad MBR signature...");
}

/*
 * Read the bootsectors and check whether the signature 
 * is valid.
 */ 
int get_boot_sector (ULONG lba, ULONG extlba, int drive_num)
{
	int i;
	UCHAR buf[512];

	temp->next = (struct bootsector *)kmalloc (sizeof (struct bootsector));
	temp = temp->next;
	temp->next = NULL;

	if(read_disk_sector (buf, drive_num, lba, 1) == IDE_ERR_NOERROR)
	{
		for (i = 0; i < 4; i ++)
			temp->part[i] = *((struct partition_entry *)&buf[446 + i*16]);
		  
		temp->signature = *((USHORT *)&buf[510]);
		if (temp->signature == MBR_SIGNATURE)
			save_part_props (&temp->part[0], lba);
		else
//			kprintf("\nget_boot_sector : Bad signature...");
			return BAD_SIG;
	}
	else return DISK_READ_ERR;
//		kprintf("\n get_boot_sector : read_sector failed ");

	if (temp->part[1].type)
		get_boot_sector (temp->part[1].start_lba + extlba, extlba, drive_num);

}

/*
 * Check if the partition is  DOS extended , Win95 extended  or Linux extended.
 */
void check_if_extended (struct partition_entry *pe, int drive_num)
{
	ULONG extlba;			/* LBA value for the extended partition */
	
	if (pe->type == PART_DOS_EXTENDED || 
			pe->type == PART_WIN95_EXTENDED || 
			pe->type == PART_LINUX_EXTENDED)
	{
		extlba = pe->start_lba;
		get_boot_sector (pe->start_lba, extlba, drive_num);
	}
}

void traverse_extended_partitions (int drive_num)
{	
	temp = bslist;
	check_if_extended (&temp->part[0], drive_num);
	check_if_extended (&temp->part[1], drive_num);
	check_if_extended (&temp->part[2], drive_num);
	check_if_extended (&temp->part[3], drive_num);
}

void read_disk_partitions ()
{
	int drive_num;
	part_in = 1;
	partition = (struct part_props *)kmalloc(sizeof(struct part_props));	
	part_temp = partition;
	part_temp->next = NULL;
	for (drive_num = 0; drive_num < num_ide_drives; drive_num ++)
	{
		read_mbr (drive_num);
		traverse_primary_partitions ();
		traverse_extended_partitions (drive_num);
	}
}
