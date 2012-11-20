#include <shell_builtin.h>

char * file_type[] = {" Unknown", " File", " Directory", " Char Device", " Blk Device", " Pipe", " Socket", " Sym Link" };

/*	Executes the help command	*/
int cmd_help(int argc, char **argv)
{	int elements;
	int foo, bar, good;
	sbuiltin *cmds = shell_cmdtab(&elements);
	if (argc <= 1) {
		kprintf("%d commands:\n", elements);
		while (elements) {
			kprintf("%s\n", cmds[--elements].name);
		}
	} else {
		foo = argc - 1;
		while (foo >= 1) {
			good = 0;
			bar = elements - 1;
			while (bar >= 0) {
				if (strcmp(argv[foo], cmds[bar].name) == 0) {
					good = 1;
					break;
				}
				bar--;
			}
			if (good)
				kprintf("%s\n", cmds[bar].help);
			else
				kprintf("Command \"%s\" not found.\n", argv[foo]);
			foo--;
		}
	}
	return 0;
}

/*	Executes the ls command and also decides to list the directories of which FS	*/
int cmd_ls(int argc, char **argv)
{
	int count, k;
	char *c;
	fs_temp = fs_prop;
	if ( fs_temp == NULL ) kprintf("Partition not supported\n");
	else if( argc == 1 )	{
		while ( fs_temp != NULL )
		{
			kprintf("%s ", fs_temp->filename);
			fs_temp = fs_temp->next;
		}
		kprintf("\n ");
	}
	else if ( argc == 2 )	{
		c = argv[1];
		if ( *c == '-')
		{	
			c++;
			if ( *c == 'l' )
			{
				while ( fs_temp != NULL )
				{
					if (strcmp(fs_temp->filename,""))
					kprintf("%-15s %-10d %-06d %-20s\n", 
								file_type[fs_temp->file_type],
								fs_temp->size,
								fs_temp->start,
								fs_temp->filename);
					fs_temp = fs_temp->next;
				}
			}
			else kprintf ( "Wrong switch used: See help ls\n");
		}	
		else kprintf ("Wrong usage of command : See help ls\n");
	}
	else kprintf ("Wrong usage of command : See help ls\n");
}

/*	Clears screen	*/
int cmd_clear(int argc, char **argv)
{
	kclear_screen();
}

/*	Reboots system	*/
int cmd_reboot(int argc, char **argv)
{
	reboot();
}

/*	Change control to any directory in the current partition	*/
int cmd_cd(int argc, char **argv)
{
	char *c;
	if (argc == 2) 
	{
		fs_temp = (struct fs_properties *) fs_chdir( argv[1] );
		if ( shell_panic (fs_temp)) 
		{
			fs_temp = (struct fs_properties *) fs_chdir (".");
			return FAILURE;
		}
		fs_prop = fs_temp;		
	}
	else kprintf ( "Wrong switch used: See help cd\n");
}

/*	Prints the partition table	*/
int cmd_ppart()
{
	part_temp = partition;
	kprintf ("Partition      Type          Starting LBA         Patition Size\n ");
	while (part_temp->next != NULL)
	{
	kprintf("   %d     %10s            %-10d          %-6dMb\n ", part_temp->name, 
							part_temp->type,
								part_temp->st_lba,
								 part_temp->part_size);
	part_temp = part_temp->next;
	}
return SUCCESS;
}

/*	Displays the contents of a file on the screen, passes control to the coresponding driver */
int cmd_more( int argc, char **argv )	
{	
	UCHAR *load_mem;
	int size, i, found = 0 ;
	if( argc > 2 ) kprintf("Wrong usage of the command: See help more\n");
	load_mem = (UCHAR *)0x0000;	
	load_mem = fs_loadfile ( argv[1], load_mem);
	if (shell_panic(load_mem)) return (FAILURE);
	fs_more = (struct fs_properties *) fs_chdir (".");
	fs_temp  = fs_more;
	while ( fs_temp != NULL )
	{
		if (!strcmp(fs_temp->filename, argv[1]) && fs_temp->file_type )
		{	
			size = fs_temp->size;
			found = 1;
			break;
		}
	fs_temp = fs_temp->next;
	}
	for ( i = 0; i < size; i++)
		kprintf ( "%c", *load_mem++ );
	return SUCCESS;
}

/*	Loads the boot sector or a specified sector	*/
int cmd_load( int argc, char **argv )
{
	char *c, *d;
	UCHAR *load_mem;
	int found = 0;
	c = argv[1];
	if ( *c == 'p')
	{
		c++;
		part_temp = partition;
		while ( part_temp->next != NULL && !found )
		{
			if ( part_temp->name == atoi(c))
			{
				found = 1;
				load_mem = (UCHAR *)0x7C00;
				read_disk_sector ( load_mem, current_drive, part_temp->st_lba, 1 );
			}
			part_temp = part_temp->next;
		}
	}
	else if ( *c == '-')
		{
			strcat( argv[1], argv[2] );
			c++;
			if ( *c == 's' )
			{
				c++;
				d = argv[3];
				load_mem = (UCHAR *)atoi(d);
				if( read_disk_sector ( load_mem, current_drive, atoi(c), 1 ) ==
 									IDE_ERR_IO_ERROR )
					shell_panic(DISK_READ_ERR);
				if(!atoi(c))
					pto_rmode ();
			}
			else if ( *c == 'f' )
				{
					c++;
					d = argv[3];	
					load_mem = (UCHAR *)atoi(d);	///change

					load_mem = fs_loadfile ( c, load_mem);
					if (shell_panic((int)load_mem)) return (FAILURE);
				}
					else kprintf ("Wrong usage of switch: See help load\n");
		}
	return SUCCESS;
}

int cmd_ldmod( int argc, char **argv )
{
	UCHAR * load_mem;
	char *d;
	d = argv[2];
	load_mem = (UCHAR *)atoi(d);
	load_mem = fs_loadfile ( argv[1], load_mem);
	if (shell_panic(register_module (load_mem)))
	return (FAILURE);
}
 
		
int cmd_root (int argc, char **argv)
{
	char *c;
	c = argv[1];
		
	if ((*c == 'p') && (argc <= 2))
	{
		c ++;
		if (fs_root (atoi (c)) == FS_DRV_NOT_FOUND)
			kprintf ("Filesystem not supported\n");
	}
	else
		kprintf ("Wrong usage of command : see help root\n");
}

void cmd_lsdrv ()
{
	list_drivers ();
}

void cmd_rmdrv (int argc, char **argv)
{
	int ret;
	if (argc == 2)
	{
		ret = unregister_module (argv[1]);
		if (ret == FS_MODULE_NOT_FOUND)
			kprintf ("Module %s not found\n", argv[1]);
	}
	else
		kprintf ("Wrong usage of command : see help rmdrv\n");
	
}

void cmd_chload()
{
//	unsigned char* bios_bootaddr = (unsigned char*)0x7C00;
	
	pto_rmode ();
}

/* Dumps the values at any memory location */
int cmd_dump( int argc, char **argv )
{
	char *c;
	int i;
	UCHAR * dump_mem;
	c = strcat (argv[1], argv[2]);
	if ( *c == '-')
	{
		c++;
		if ( *c == 'x')
		{
			c++;
			dump_mem = (UCHAR *) atoi(c);	
			kprintf("\n");
			for (i = 1; i <= 512; i++)
			{
				kprintf("%02x", *dump_mem++);
				if (!(i % 4)) kprintf(" ");	
				if (!(i % 32)) kprintf("\n");	 
			}	
		kprintf("\n");
		}
		else if ( *c == 'c' )
		{
			c++;
			dump_mem = (UCHAR *) atoi(c);
			kprintf("\n");
				for (i = 1; i <= 512; i++)
					kprintf("%c", *dump_mem++);
			kprintf("\n");
		}
		else kprintf("Wrong usage of command : See help dump\n");
	}	
}

int cmd_send ()
{
	send();
}

int cmd_receive ()
{
	receive();
}
