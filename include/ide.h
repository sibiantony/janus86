/*
 *	ATA ( IDE ) driver.
 *	References :	ATA/ATAPI-4 specs 
			http://www.t13.org/
 */

#ifndef __IDE_H
#define __IDE_H

#include "x86.h"
#include "keyboard.h"
#include "types.h"
#include "video.h"

/* IDE Control block registers */
#define IDE_DEVICE_CONTROL 	0x3F6
#define IDE_DRIVE_ADDRESS	0x3F7

/* IDE Command block registers */
#define IDE_DATA_PORT		0x1F0
#define IDE_ERROR_REG		0x1F1
#define IDE_SECTOR_COUNT	0x1F2
#define IDE_SECTOR_NUMBER	0x1F3
#define IDE_CYLINDER_LOW	0x1F4
#define IDE_CYLINDER_HIGH	0x1F5
#define IDE_DRIVE_HEAD		0x1F6
#define IDE_COMMAND		0x1F7
#define IDE_STATUS		0x1F7

/* Commands received in the command register */
#define IDE_CMD_DIAGNOSTIC	0x90
#define IDE_CMD_IDENT_DRIVE	0xEC
#define IDE_CMD_FORMAT_TRACK	0x50
#define IDE_CMD_READ_BUFFER	0xE4
#define IDE_CMD_READ_DMA	0xC9
#define IDE_CMD_READ_SECTORS	0x21
#define IDE_CMD_RS_RETRY	0x20
#define IDE_CMD_STANDBY		0x96
#define IDE_CMD_WRITE_BUFFER	0xE8
#define IDE_CMD_WRITE_DMA	0xCB
#define IDE_CMD_WRITE_SECTORS	0x31
#define IDE_CMD_WS_RETRY	0x30

/* Bits of Device Control Register */
#define IDE_DCR_NOINTERRUPT	0x02
#define IDE_DCR_RESET		0x04

/* bits of Status Register */
#define IDE_STATUS_BUSY			0x80
#define IDE_STATUS_READY		0x40
#define IDE_STATUS_WRITE_FAULT		0x20
#define IDE_STATUS_SEEK_COMPLETE	0x10
#define IDE_STATUS_DATA_REQUEST		0x08
#define IDE_STATUS_CORRECTED_DATA	0x04
#define IDE_STATUS_INDEX		0x02
#define IDE_STATUS_ERROR		0x01

/* Diagnostics code in the error register */
#define IDE_ERR_NOERROR		0x01
#define IDE_ERR_FORMATTER	0x02
#define IDE_ERR_SECTOR_BUFFER	0x03
#define IDE_ERR_ECC_CIRCUITRY	0x04
#define IDE_ERR_CONTROLLER	0x05	

/* IDE Drive values */
#define IDE_DRIVE0		0xA0
#define IDE_DRIVE1		0xB0

int read_sector(char *, int, unsigned int, unsigned int, unsigned int, unsigned int );
void init_ide(void);

#endif
