
asm (
	".code32;"
	".word	0xEF86;"
	".word 	module_end - module_start - 1;"
	".long	0x20000;"
	".long	module_start;"
	".long  module_end;"
	".byte	0x0C;"
	".string \"vfat\";"
	"module_start: ;"
);

#include<fat32.h>

int fat_item_count=0;			
int fat_nam_index;

int error( int errcode )
{
	if ( errcode <= FIL_NOT_FOUND && errcode >= NO_SUCH_PART )
		return ( SUCCESS );
	else	return ( FAILURE );
}

/*	Initialising the FAT32 read, on providing the cluster number and the lba value	*/
int init_fat32 (int drive_num,  UINT lba, UINT cluster )
{
	int errmsg;
	fat_item_count = 0;

	fat_file_prop = (struct fat_file_properties *)kmalloc(sizeof(struct fat_file_properties));
	fat_temp = fat_file_prop;
	fat_temp->next = NULL;
	if ( error (errmsg= fat32_rd_bootsec( drive_num, lba )))	return (errmsg);
	if ( error (errmsg = fat32_rd_cluster( drive_num, lba, cluster))) return (errmsg);
	fat_temp = fat_file_prop;
	return(fat_item_count);
}

/*	To load a file */
UCHAR * load_fat32 ( char *c, UCHAR * load_mem)
{
	int count, found = 0;
	int k;
	count = init_fat32 (current_drive, current_lba, current_cluster);
	while ( fat_temp != NULL && !found)
	{
		if ( !strcmp( c, (char *) fat_temp->filename))
		{	
			if ( /*!(fat_temp->file_type & FAT_ATTR_DIRECTORY || 
						fat_temp->file_type & FAT_ATTR_VOLUME_ID)*/
					fat_temp->file_type != DIRECTORY )
				{
					found = 1;
					if (!load_mem) 
						load_mem = (UCHAR *)kmalloc(fat_temp->size);
					fat32_mmap( current_drive, 
							current_lba, 
							fat_temp->st_clust, 
							load_mem,
							fat_temp->size );
					return (load_mem);
				}
			else 	return((UCHAR*)NOT_REG_FILE);
		}
	fat_temp = fat_temp->next;
	}
	if(!found) return((UCHAR*)FIL_NOT_FOUND);
}

/*	Resolves the path of the cd command for FAT32 FS; control lands up here from cmd_cd	*/
struct fat_file_properties * chdir_fat32( char *c)
{	
	int args, j, k, count, found=0;
	static int prev_cluster = 2;
	static char	*argtable[DIRECTORY_NUM+1];
	static char sc[PATH_LEN+1];
	char *ccp = &sc[0];
	strcpy(ccp, c);
	args = 0;
	while (*ccp) {
		if (args >= DIRECTORY_NUM) {
			return ((struct fat_file_properties *)PAT_LEN_EXCEED);
		}
		argtable[args++] = ccp;
		while (*ccp && !(*ccp == '/'))
			ccp++;
		while (*ccp == '/' || *ccp == '\n')
			*ccp++ = '\0';
	}
	argtable[args] = NULL;

	count =	init_fat32 (current_drive, current_lba, current_cluster);
	
	if (!strcmp (c, "."))	{
			init_fat32 (current_drive, current_lba,current_cluster);
			return ((struct fat_file_properties *)fat_file_prop);
	}
	for (j = 0; j < args; j++)
	{
		if (!strcmp(argtable[j],""));
		else	{ 
		while ( fat_temp != NULL && !found)
		{	
			if (!strcmp( (char*)fat_temp->filename, argtable[j]) )
				{
					if( fat_temp->file_type == DIRECTORY )
					{
						prev_cluster = current_cluster;
						current_cluster = fat_temp->st_clust;
						found = 1;
						count =	init_fat32 (current_drive, 
									current_lba, 
									current_cluster);
						break;
					}
					else	return ((struct fat_file_properties *)CD_ERR);
				}
		fat_temp = fat_temp->next;
		}
			if(found); 
			else 	{ 
//			current_cluster = prev_cluster;
			count =	init_fat32 (current_drive, 
						current_lba, 
						current_cluster);
			return ((struct fat_file_properties *)DIR_NOT_FOUND);
			}
			found=0;
		}
	}
	fat_item_count = 0;
	return (fat_file_prop); 
}

/*	Checks whether the character that occurs in the name is valid	*/
void Check_char(char c, int lng)
{
	if(!lng)
	{
		if((c != ' ') && (c != 0x00 ))
		fat_temp->filename[fat_nam_index++] = c;
	}
	else 	{
		if ( c == ' ' )	{
			fat_temp->filename[fat_nam_index++] = '\\';
			fat_temp->filename[fat_nam_index++] = c;
		}
		else fat_temp->filename[fat_nam_index++] = c;
	}
}

/*	Generates the name of the long file name directories	*/
int Generate_Long_Name ( int drive_num, UINT lba, UINT cluster_no, UCHAR long_direntry[], int i, int k)
{	
	int nam_in, flag=0;
	fat_nam_index = 0;
	do	{
			if( i <= 0 )	{
			flag = 1;
			i = DIRENT_PER_SECTOR;
			k = k - 1;
			if( read_disk_sector(long_direntry, drive_num, 
							    lba+CLUSTER_TO_LBA(cluster_no)+k,    								    1) == IDE_ERR_IO_ERROR )	
				return(DISK_READ_ERR);
		}			

		fat32longdir = (struct fat_long_dirent *)&long_direntry[(i-1)*32];

		for( nam_in = 0; nam_in < 10; nam_in+=2)
		{
			Check_char( fat32longdir->LDIR_Name1[nam_in], LONG_NAME);
		}
		for( nam_in = 0; nam_in < 12; nam_in+=2)
		{
			Check_char( fat32longdir->LDIR_Name2[nam_in], LONG_NAME);
		}
		for( nam_in = 0; nam_in < 4; nam_in+=2)
		{
			Check_char( fat32longdir->LDIR_Name3[nam_in], LONG_NAME);
		}
		i--;

	}		while ((fat32longdir->LDIR_Ord & 0x40)==0 && i>=0);
	if(flag) {
		k = k + 1;
		if( read_disk_sector(long_direntry, drive_num, 
					    lba+CLUSTER_TO_LBA(cluster_no)+k,    						    1) == IDE_ERR_IO_ERROR )	
			return(DISK_READ_ERR);
	}
	fat_temp->filename[fat_nam_index] = '\0';
	fat_temp->next = (struct fat_file_properties *)kmalloc(sizeof(struct fat_file_properties));
	fat_temp = fat_temp->next;
	fat_temp->next = NULL;
	fat_item_count++;	

	return;	
}


/*	Reads the FAT and decides the next cluster where the data corresponding to the file is 		stored					*/
int fat32_rd_FAT(int drive_num, UINT lba, UINT cluster)
{
	UCHAR FAT_entry[512];
	UCHAR direntry[512];
	int i;
	UINT sec_no;
	UINT offset;
	int clu_no;
	sec_no = cluster / CLU_PER_FAT;
	offset = cluster % CLU_PER_FAT; 
	if( read_disk_sector(FAT_entry, drive_num, fat_begin_lba + sec_no, 1) == IDE_ERR_IO_ERROR )
		return (DISK_READ_ERR);		
	FAT = (struct FAT_32bit_entry *)FAT_entry;
	if(FAT->fat32bit[offset]==FAT_EOF_MARKER) return(0);
	else return(FAT->fat32bit[offset]);
}

/*	Reads the bootsector of the FAT32 FS	*/
int fat32_rd_bootsec(int drive_num, UINT lba)
{
	UCHAR bootsec[512], i;
	if( read_disk_sector(bootsec, drive_num, lba, 1) == IDE_ERR_IO_ERROR )
		return(DISK_READ_ERR);
	fat32boot = (struct fat_bootsector *)bootsec;
	if ( fat32boot->magic != 0xAA55 )
		return ( BAD_FAT_BOOT_SECTOR );
	
	fat_begin_lba = lba + fat32boot->num_boot_sectors;

	cluster_begin_lba = fat32boot->num_boot_sectors + 
				( fat32boot->num_fats * fat32boot->big_sectors_per_FAT );
	root_dir_first_cluster = fat32boot->root_directory_start;
	sectors_per_cluster = fat32boot->sectors_per_cluster;
	return SUCCESS;
}

/*	Reads any cluster of the FAT32 FS	*/
int fat32_rd_cluster ( int drive_num, UINT lba, UINT cluster_no )
{
	UCHAR direntry[512];
	UINT next_cluster, size;

	int i = 0,k = 0, root_cluster, j, prev = 0x00;
	UINT start_cluster  ;

	for(k=0;k<sectors_per_cluster;k++)
	{
		if( read_disk_sector(direntry, drive_num, lba + CLUSTER_TO_LBA(cluster_no) 
								+ k, 1) == IDE_ERR_IO_ERROR )
			return (DISK_READ_ERR);

		for( i = 0; i < DIRENT_PER_SECTOR; i++ )
		{
			fat32dir = (struct fat_dirent *)&direntry[i*32];
			if ( fat32dir->attrib != 0x0F && fat32dir->attrib != 0x08 )
			{	
				start_cluster = fat32dir->st_clust_msw;
				start_cluster = start_cluster << 16 ; 
				start_cluster =	start_cluster + fat32dir->st_clust;
				if (!start_cluster && fat32dir->attrib == 0x10) start_cluster += 2;

				/*	filling in the properties of the FAT32 diectory	*/
				if( fat32dir->attrib & FAT_ATTR_DIRECTORY || 
						fat32dir->attrib & FAT_ATTR_VOLUME_ID ) 
					{fat_temp->file_type = DIRECTORY;}
				else if ( fat32dir->attrib & FAT_ATTR_FILE ) 
					fat_temp->file_type = REG_FILE;
				else fat_temp->file_type = fat32dir->attrib;
				fat_temp->st_clust = start_cluster;
				fat_temp->size = fat32dir->file_size;

				d_date = (struct dos_date *)&fat32dir->mdate;
				d_time = (struct dos_time *)&fat32dir->mtime;
				if(prev == 0x0F)
				{
					if(!i) {
					if( read_disk_sector(direntry, drive_num, 
								lba + CLUSTER_TO_LBA(cluster_no) 
								+ k - 1, 1) == IDE_ERR_IO_ERROR )
						return (DISK_READ_ERR);				
					i = DIRENT_PER_SECTOR;				
					Generate_Long_Name(drive_num,
							lba,
							cluster_no,
							direntry, i, k);
					if( read_disk_sector(direntry, drive_num,
								lba + CLUSTER_TO_LBA(cluster_no) 
								+ k, 1) == IDE_ERR_IO_ERROR )
						return (DISK_READ_ERR);
					i = 0x00;
					}
					else Generate_Long_Name(drive_num,
								lba,
								cluster_no,
								direntry, i, k);
					prev = 0x00;
				}
				else
				{	
					if(fat32dir->name[0]!=0 && fat32dir->name[0]!=0xE5 
							&& fat32dir->name[0]!=0x20) 						{
						fat_nam_index = 0;
						for( j = 0; j < 8; j++) 
							Check_char( fat32dir->name[j], 
								SHORT_NAME );
						if( fat32dir->ext[0] != ' ' ) 
							fat_temp->filename[fat_nam_index++] = '.';
						for( j = 0; j < 3; j++) 
							Check_char( fat32dir->ext[j],
										SHORT_NAME );
						fat_temp->filename[fat_nam_index] = '\0';
						fat_item_count++;
						fat_temp->next = (struct fat_file_properties *)
										kmalloc
							(sizeof(struct fat_file_properties));
						fat_temp = fat_temp->next;
						fat_temp->next = NULL;
					}
				}
			
			}
			if(fat32dir->name[0]!=0 && fat32dir->name[0]!=0xE5 
							&& fat32dir->name[0]!=0x20) 					prev = fat32dir->attrib;			
		}
	}
	next_cluster=fat32_rd_FAT(drive_num, lba, cluster_no);
	if (!error(next_cluster))
	{
		if (next_cluster)
			fat32_rd_cluster( drive_num, lba, next_cluster);
	}
	else return next_cluster;
	strcpy ( (char*)fat_temp->filename,"");
		
	return SUCCESS;
}

/*	Loads the contents of the file to any specified memory address	*/
int fat32_mmap( int drive_num, UINT lba, UINT cluster_no, UCHAR *mem_loc, int size )
{
	UCHAR mmap_data[512];
	int next_cluster;
	int k,i;
	for ( k = 0; k <= ((size-1)/512); k++ ) 
	{	
		if( read_disk_sector(mmap_data, drive_num, lba + CLUSTER_TO_LBA(cluster_no) 
								+ k, 1) == IDE_ERR_IO_ERROR )
			return (DISK_READ_ERR);
		
		for(i = 0; i < 512; i++ ) *mem_loc++ = mmap_data[i];
	}
	next_cluster=fat32_rd_FAT(drive_num, lba, cluster_no);
	if (!error(next_cluster))
	{
		if (next_cluster)
			fat32_mmap( current_drive, current_lba, next_cluster, mem_loc, (size-512));
	}
	else return next_cluster;
	return;
}

struct fat_file_properties * root_fat32 (int lba, int drive, int cluster_no)
{
	current_lba = lba;
	current_drive = drive;
	current_cluster = cluster_no;
	fat_file_prop = chdir_fat32 ("/");
	return (fat_file_prop);

}

asm (
	"module_end: ;"
	".string \"mod_root   \" ;"
	".long	root_fat32;"
	".string \"mod_chdir  \" ;"
	".long	chdir_fat32;"
	".string \"mod_loadf  \" ;"
	".long	load_fat32;"
);  
