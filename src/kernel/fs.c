/*
	fs.c - Here we accept the request for a root filesystem.
	Also,  initiliazes the function pointers for individual
	file system modules
*/
#include <fs.h>

/* function pointers that accepts the driver symbol values */ 
int (*drv_root) (int, int, int);
struct fs_properties* (*drv_chdir) (char *);
int (*drv_loadfile) (char * ,unsigned char *);

/* initlialises the fs module structure and .. 
*/
int init_fs_modules ()
{
	fs_drv_reg *tmp;
	
	tmp = (fs_drv_reg *)kmalloc (sizeof(fs_drv_reg));
#ifdef EXT2_COMPILED_STATIC
	extern ext2_root;
	extern ext2_chdir;
	extern ext2_loadfile;
	tmp->fs_type_byte = PART_LINUX_NATIVE;
	tmp->load_mem = (unsigned long)NULL;
	strcpy (tmp->drv_name, EXT2_DRV_NAME);
	tmp->sym_root = (unsigned long)&ext2_root;
	tmp->sym_chdir = (unsigned long)&ext2_chdir;
	tmp->sym_loadfile = (unsigned long)&ext2_loadfile;
	tmp->drv_next = NULL;
#endif
	fs_drv_list = tmp;
	return 0;
	
}

int fs_root (int partition_num)
{
	fs_drv_reg *tmp;

/* Get the partition type byte corresponding to partition_num */
	part_temp = partition;
	while (part_temp->next != NULL)
	{
		if (part_temp->name == partition_num)
			break;
		part_temp = part_temp->next;
	}

/* Look for available file system drivers that can handle 
 * the requested partition. If found assign the functions with their symbol values 
 */
	tmp = fs_drv_list;
	while (tmp != NULL)
	{
		if (tmp->fs_type_byte == part_temp->type_byte)
		{
			drv_root = (int (*) (int, int, int))tmp->sym_root;
			drv_chdir = (struct fs_properties* (*) (char *))tmp->sym_chdir;
			drv_loadfile = (int (*) (char * ,unsigned char *))tmp->sym_loadfile;
			fs_prop = (struct fs_properties*)(*drv_root)(part_temp->st_lba, 0, 2);
			return FS_DRV_FOUND;
		}
		tmp = tmp->drv_next;
	}
	return FS_DRV_NOT_FOUND;
	
}

/* Register a newly loaded module. Identify which 
	module does what and save the symbol values	*/
int register_module (unsigned char *load_addr)
{
	fs_drv_reg *new_mod;
	fs_drv_reg *tmp;

	fs_drv_mod *new_mod_header; 
	fs_sym_tab *new_mod_sym[FS_EXPORTED_NUM];
	int i = 0;

	new_mod_header = (fs_drv_mod *)kmalloc (sizeof(fs_drv_mod));
	memcpy (new_mod_header, load_addr, sizeof(fs_drv_mod));
	
	if (new_mod_header->mod_sign == 0xEF86)
	{	
		kprintf ("\nFound FS module with signature : 0x%X", 
				new_mod_header->mod_sign);
		kprintf ("\nModule name : %s\n",new_mod_header->mod_name );
	}
	else
		return FS_BAD_MODULE_FMT;	
	new_mod = (fs_drv_reg *)kmalloc (sizeof(fs_drv_reg));

	for (i = 0; i < FS_EXPORTED_NUM; i++)
	{
		new_mod_sym[i] = (fs_sym_tab *)kmalloc
					(sizeof (fs_sym_tab));
		memcpy (new_mod_sym[i], 
			(const char*)(new_mod_header->sym_tab_addr + i * sizeof(fs_sym_tab)),
					 sizeof(fs_sym_tab));
	}

	new_mod->fs_type_byte 	= new_mod_header->fs_type_byte;
	new_mod->load_mem 	= new_mod_header->load_mem;
	strcpy (new_mod->drv_name, new_mod_header->mod_name);
	new_mod->sym_root 	= new_mod_sym[0]->sym_val;
	new_mod->sym_chdir 	= new_mod_sym[1]->sym_val;
	new_mod->sym_loadfile 	= new_mod_sym[2]->sym_val;
	new_mod->drv_next 	= NULL;

	if (fs_drv_list == NULL)
		fs_drv_list = new_mod;
	else {
		tmp = fs_drv_list;
		while (tmp->drv_next != NULL)
			tmp = tmp->drv_next;
		tmp->drv_next = new_mod;
	}
	return FS_MODULE_REGISTERED;
}

/* delete the module entry  from the 
	list	*/
int unregister_module (char *drv_name)
{
	fs_drv_reg *tmp, *prev_drv;
	
	tmp = fs_drv_list;
	if (strcmp(tmp->drv_name, drv_name) == 0)
	{
		fs_drv_list = fs_drv_list->drv_next;
		kfree (tmp);
		return FS_MODULE_REMOVED;
	}
	while (tmp != NULL)
	{
		if (strcmp(tmp->drv_name, drv_name) == 0)
		{
			prev_drv->drv_next = tmp->drv_next;
			kfree (tmp);
			drv_root = (int (*) (int, int, int))prev_drv->sym_root;//NULL;
			drv_chdir = (struct fs_properties* (*) (char *))prev_drv->sym_chdir;//NULL;
			drv_loadfile = (int (*) (char * ,unsigned char *))prev_drv->sym_loadfile;
			return FS_MODULE_REMOVED;
		}
		prev_drv = tmp;
		tmp = tmp->drv_next;
	}
	return FS_MODULE_NOT_FOUND;
}

void list_drivers ()
{
	fs_drv_reg *tmp;
	
	tmp = fs_drv_list;
	while (tmp != NULL)
	{
		kprintf ("Name : %s  FS type : 0x%X  Mem : 0x%X\n", tmp->drv_name, 
				tmp->fs_type_byte, tmp->load_mem);
		tmp = tmp->drv_next;
	}
	kprintf ("\n");
}
/* invoke the chdir routine for the filesystem
	requested through fs_root */
struct fs_properties* fs_chdir (char *path)
{
	if (drv_chdir != NULL)
		return ((*drv_chdir)(path));
	else
		return FS_DRV_NOT_FOUND;
}

/* invoke the loadfile routine for the filesystem
	requested through fs_root */
int fs_loadfile (char *filename, unsigned char *load_mem)
{
	if (drv_loadfile != NULL)
		return ((*drv_loadfile)(filename, load_mem));
	else
		return FS_DRV_NOT_FOUND;
}
