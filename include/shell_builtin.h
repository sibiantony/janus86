#ifndef __SHELL_BUILTIN_H
#define __SHELL_BUILTIN_H

//edit
#include "kprintf.h"
#include "types.h"
#include "diskio.h"
#include "fs.h"
#include "shell.h"

#include "serial.h"


//Searches the LBA value of a partition
int search_part_name (int );

// Help for each command
int cmd_help(int , char **);

// List the contents of a directory
int cmd_ls(int argc, char **argv);

// Clears the screen
int cmd_clear(int argc, char **argv);

// Reboots the system
int cmd_reboot(int argc, char **argv);

// Change directory
int cmd_cd(int argc, char **argv);

// Prints the Partition table
int cmd_ppart();


int cmd_more( int , char ** );

/* Load a disk sector or partition bootsector */
int cmd_load( int , char **);

/* Perform a chainlod to other system */
void cmd_chload ();

/* Request for a root filesystem */
int cmd_root (int , char **);

/* List the available FS drivers as well as the loaded modules*/
void cmd_lsdrv ();

/* Remove an existing module from the list	*/
void cmd_rmdrv (int, char **);

/* Dumps the values at any memory location */
int cmd_dump( int , char ** );

/* loads a module at a specified memory location */
int cmd_ldmod( int, char **);

int cmd_send();

int cmd_receive();
#endif
