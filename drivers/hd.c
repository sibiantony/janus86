/*
 *	Hard disk driver according to the ATA spec .
 *	References : ATA/ATAPI-4 spec.  http://www.t13.org	
 *		IDE - Hardware Reference & Information Document http://www.repairfaq.org	
 *
 */
#include <hd.h>

/*
 * Read a block at the logical block number indicated.
 */
int read_sector(unsigned char *buf, int drive_num, unsigned int lba,
		unsigned int num_sectors)
{
	int i;
	unsigned short *wordbuf;

	/* Initialize the command block registers */
	outportb(IDE_SECTOR_COUNT, num_sectors);
	outportb(IDE_SECTOR_NUMBER, LBA_LOW_BYTE(lba));
	outportb(IDE_CYLINDER_LOW, LBA_HIGH_BYTE(lba));
	outportb(IDE_CYLINDER_HIGH, LBA_EXTENDED_LOW_BYTE(lba));

	if (drive_num == 0)
		outportb(IDE_DRIVE_HEAD,
			 IDE_DRIVE0 | LBA_EXTENDED_HIGH_BYTE(lba));
	else if (drive_num == 1)
		outportb(IDE_DRIVE_HEAD,
			 IDE_DRIVE1 | LBA_EXTENDED_HIGH_BYTE(lba));

	/* Load command register with the read command code     */
	outportb(IDE_COMMAND, IDE_CMD_READ_SECTORS_RETRY);

	while (inportb(IDE_STATUS) & IDE_STATUS_BUSY)	/* wait for the drive */
		;

	if (inportb(IDE_STATUS) & IDE_STATUS_ERROR)
		return IDE_ERR_IO_ERROR;

	wordbuf = (unsigned short *) buf;
	for (i = 0; i < 256 * num_sectors; i++)
		wordbuf[i] = inportw(IDE_DATA_PORT);

	return IDE_ERR_NOERROR;
}


/*
 * write a block at the logical block number indicated.
 */
int write_sector(unsigned char *buf, int drive_num, unsigned int lba,
		 unsigned int num_sectors)
{
	int i;
	unsigned short *wordbuf;

	/* Initialize the command block registers */
	outportb(IDE_SECTOR_COUNT, num_sectors);
	outportb(IDE_SECTOR_NUMBER, LBA_LOW_BYTE(lba));
	outportb(IDE_CYLINDER_LOW, LBA_HIGH_BYTE(lba));
	outportb(IDE_CYLINDER_HIGH, LBA_EXTENDED_LOW_BYTE(lba));

	if (drive_num == 0)
		outportb(IDE_DRIVE_HEAD,
			 IDE_DRIVE0 | LBA_EXTENDED_HIGH_BYTE(lba));
	else if (drive_num == 1)
		outportb(IDE_DRIVE_HEAD,
			 IDE_DRIVE1 | LBA_EXTENDED_HIGH_BYTE(lba));

	/* Load command register with the write command code    */
	outportb(IDE_COMMAND, IDE_CMD_WRITE_SECTORS_RETRY);

	while (inportb(IDE_STATUS) & IDE_STATUS_BUSY)	/* wait for the drive */
		;

	wordbuf = (unsigned short *) buf;
	for (i = 0; i < 256 * num_sectors; i++)
		wordbuf[i] = outportw(IDE_DATA_PORT);	/* wait for the drive */


	while (inportb(IDE_STATUS) & IDE_STATUS_BUSY);

	if (inportb(IDE_STATUS) & IDE_STATUS_ERROR)
		return IDE_ERR_IO_ERROR;

	return IDE_ERR_NOERROR;
}

/*
 * Gather info about the ide drives present.
 */
int probe_ide_drives(void)
{
	int i;
	int status;
	short buffer[256];
	int drive;

	num_ide_drives = 0;
	for (drive = 0; drive < 2; drive++) {
		outportb(IDE_DRIVE_HEAD,
			 (drive == 0) ? IDE_DRIVE0 : IDE_DRIVE1);
		outportb(IDE_COMMAND, IDE_CMD_IDENT_DRIVE);
		while (inportb(IDE_STATUS) & IDE_STATUS_BUSY);

		status = inportb(IDE_STATUS);
		/*
		 * simulate failure
		 * status = 0x50;
		 */
		if ((status & IDE_STATUS_DATA_REQUEST)) {
			/* drive responded to ATA probe */
			for (i = 0; i < 256; i++)
				buffer[i] = inportw(IDE_DATA_PORT);

			ide_drive[drive].num_cylinders =
			    buffer[IDE_IDENT_NUM_CYLINDERS];
			ide_drive[drive].num_heads =
			    buffer[IDE_IDENT_NUM_HEADS];
			ide_drive[drive].num_sectors_per_track =
			    buffer[IDE_IDENT_NUM_SECTORS_TRACK];
			ide_drive[drive].num_bytes_per_sector =
			    buffer[IDE_IDENT_NUM_BYTES_SECTOR];
			num_ide_drives++;
		} else {
			/* try for ATAPI */
			outportb(IDE_FEATURE_REG, 0);	/* disable dma & overlap */

			outportb(IDE_DRIVE_HEAD,
				 (drive == 0) ? IDE_DRIVE0 : IDE_DRIVE1);
			outportb(IDE_COMMAND, IDE_CMD_ATAPI_IDENT_DRIVE);
			while (inportb(IDE_STATUS) & IDE_STATUS_BUSY);
			status = inportb(IDE_STATUS);
		}
	}
	if (!num_ide_drives)
		return IDE_ERR_DRIVE_NOT_FOUND;
	return 0;
}

/*
 * Initialize the IDE controller.
 */
void init_ide(void)
{
	int errorCode;

	/* Reset the controller and drives */
	outportb(IDE_DEVICE_CONTROL, IDE_DCR_NOINTERRUPT | IDE_DCR_RESET);

	outportb(IDE_DEVICE_CONTROL, IDE_DCR_NOINTERRUPT);

	while (inportb(IDE_STATUS) & IDE_STATUS_BUSY);

	outportb(IDE_COMMAND, IDE_CMD_DIAGNOSTIC);
	while (inportb(IDE_STATUS) & IDE_STATUS_BUSY);
	errorCode = inportb(IDE_ERROR_REG);

}
