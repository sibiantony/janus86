
#include <dir.h>

UINT item_index=0;
UINT nam_index=0;


/* Initializes the ext2 FS filling in the relevant values in the global variables */

int init_ext2 ( int drive_num, UINT lba, UINT inode )
{
	ext2_file_prop = (struct file_properties *)kmalloc(sizeof(struct file_properties ));
	ext2_temp = ext2_file_prop;
	ext2_temp->next = NULL;
	
	ext2_rd_superblock ( drive_num, lba);
	ext2_rd_inode( drive_num, lba, inode);
//	ext2_temp = ext2_file_prop;
	return(item_index);	 
}

/*	To load a file	*/
UCHAR * ext2_loadfile ( char *c, UCHAR * load_mem)
{
	int size, found = 0, k;
	int count;
	count =  init_ext2 (current_drive, current_lba, current_inode );
	ext2_temp = ext2_file_prop;
	
	while (ext2_temp != NULL && !found )
	{
		if ( !strcmp( c, ext2_temp->filename))
		{
			if ( ext2_temp->file_type == REG_FILE )
				{	
					if (!load_mem) load_mem = (UCHAR*)kmalloc(ext2_temp->size);
					found = 1;
					size = ext2_load_file (ext2_temp->inode,
								 load_mem,
									 ext2_temp->size);
					return (load_mem);
				}	
			else	return ( NOT_REG_FILE ); 
		}
	ext2_temp = ext2_temp->next;
	}
	if (found); else return (FIL_NOT_FOUND);
	return(FAILURE);
}

/*	This function resolves the path that is sent from 
 *	the shell to ext2/3 driver */
	
struct file_properties * ext2_chdir (char *c)
{	
	int args, j, k, count, found=0;
	static int prev_inode = 2;
	static char	*argtable[DIRECTORY_NUM+1];
	static char sc[PATH_LEN+1];
	char *ccp = &sc[0];
	strcpy(ccp, c);
	args = 0;

	while (*ccp) {
		if (args >= DIRECTORY_NUM) {
			shell_panic (PAT_LEN_EXCEED);
			return FAILURE;
		}
		argtable[args++] = ccp;
		while (*ccp && !(*ccp == '/'))
			ccp++;
		while (*ccp == '/' || *ccp == '\n')
			*ccp++ = '\0';
	}
	argtable[args] = NULL;

	count =	init_ext2 (current_drive, current_lba, current_inode);
	ext2_temp = ext2_file_prop;
	for (j = 0; j < args; j++)
	{
		if (!strcmp(argtable[j],""));
		else	{
		while ( ext2_temp != NULL && !found )
		{	
			if (!strcmp( ext2_temp->filename, argtable[j]) )
				{	
					if ( ext2_temp->file_type == DIRECTORY )
					{
						prev_inode = current_inode;
						current_inode = ext2_temp->inode;
						found = 1;
						count =	init_ext2 (current_drive, 
									current_lba, 
									current_inode);	
						ext2_temp = ext2_file_prop;
						break;
					}
					else 	{
						shell_panic(CD_ERR);
						return (ext2_file_prop);
					}
				}
		ext2_temp = ext2_temp->next;
		}
			if(found); 
			else 	{ 
//			current_inode = prev_inode;
			count =	init_ext2 (current_drive, 
						current_lba, 
						current_inode);
			ext2_temp = ext2_file_prop;
	
			shell_panic(DIR_NOT_FOUND); 
			return (ext2_file_prop);
			}
			found=0;
		}
	}
	return (ext2_file_prop);
}

/* Gets the Access Control List from the field i_mode */
void print_mode(UINT m)
{
	UINT iss,mask;
	UCHAR table[]="USGrwxrwxrwx";

	mask=0x0800;
	for(iss=0;iss<12;iss++)
    	{
		kprintf("%c", ((m&mask)?(table[iss]) :'-'));
		mask>>=1;
    	}
}

/* Reads the directory entry for current working directory */

int ext2_rd_dir ( int drive_num, UINT lba, struct ext2_inode *intab )
{
	int k, prev_rec_len, i, j;
	UCHAR dirbuf[4096];
	UCHAR indbuf[512];
	UINT inodefile;
	UINT isam;
	item_index = 0;
	nam_index = 0;

	for( k = 0; k < 12; k++)
	{
		if ( intab->i_block[k] != 0 )
		{
			prev_rec_len=0;
			ext2_rd_block( drive_num, lba + (gbinfo->bs/512) * intab->i_block[k] - 2,
						gbinfo->bs/512, dirbuf );
			do {
				direntry=(struct ext2_dir_entry_2*)&dirbuf[prev_rec_len];
				for (j=0; j < direntry->name_len; j++)
				{	
					if (direntry->name[j] != ' ') 
					ext2_temp->filename[nam_index++] = direntry->name[j];
					else {
					ext2_temp->filename[nam_index++] = '\\';
					ext2_temp->filename[nam_index++] = direntry->name[j];
					}				
				}
				ext2_temp->filename[nam_index] = '\0';
				nam_index=0;
				inodefile=inode_to_lba(direntry->inode);
				ext2_rd_block( drive_num, lba + inodefile, 1, indbuf );		
				isam = get_index(direntry->inode);
				intab1[isam]=(struct ext2_inode *)&indbuf[isam*128];

				/* filling in the file properties structure */
				ext2_temp->size = intab1[isam]->i_size;
				ext2_temp->inode = direntry->inode;
				ext2_temp->file_type = direntry->file_type;
				
				if ( direntry->rec_len + prev_rec_len >= gbinfo->bs ) goto a;
					prev_rec_len = direntry->rec_len+prev_rec_len;
				item_index++;
				
				ext2_temp->next = (struct file_properties *)
							kmalloc(sizeof(struct file_properties ));
				ext2_temp = ext2_temp->next;
				ext2_temp->next =NULL;
			} while(direntry->rec_len>0);

		}	a:;
	}

	return(1);		 
}

/* Reads the directory entry of a  file and copies it
 * to a specified memory location */

int ext2_rd_file ( UINT lba, struct ext2_inode *intab, UCHAR *memloc )
{
	UCHAR databuf[4096];
	UCHAR indirectbuf[4096];
	UCHAR indirect2buf[4096];
	int file_size;
	int indblocks;
	int index;
	int j=0, l=0, k=0;
	indblocks = gbinfo->bs / 4;
	for( k = 0; k < 12; k++ )
	{
		file_size = intab->i_size;
		if( intab->i_block[k] != 0 )
		{
			ext2_rd_block( 0, 
					lba + (gbinfo->bs/512) * intab->i_block[k] - 2,
					gbinfo->bs/512, 
					databuf );
			for(index=0;index< 4096; index++) *(memloc++) = databuf[index];
		}
	}
	
	/* For Singly Indirect Blocks */
	if( intab->i_block[12] != 0 )
	{
		
		ext2_rd_block( 0, 
				lba + (gbinfo->bs/512) * intab->i_block[12] - 2,
				gbinfo->bs/512, 
				indirectbuf );
		for( j = 0; j < indblocks; j++)
		{
			sind = (struct sindirect *)&indirectbuf[j*4];
			ext2_rd_block( 0, 
					lba + (gbinfo->bs/512) * sind->siblock - 2,
					gbinfo->bs/512, 
					databuf );
			for(index=0;index<4096; index++) *(memloc++)=databuf[index];		
		}
	}

	/* For Doubly Indirect Blocks */
	if( intab->i_block[13] != 0 )
	{
		
		ext2_rd_block( 0, 
				lba + (gbinfo->bs/512) * intab->i_block[13] - 2,
				gbinfo->bs/512, 
				indirectbuf );
		for( j = 0; j < indblocks; j++)
		{
			sind = (struct sindirect *)&indirectbuf[j*4];
			ext2_rd_block( 0, 
					lba + (gbinfo->bs/512) * dind->diblock - 2,
					gbinfo->bs/512, 
					indirect2buf );
			for( l = 0; l < indblocks; l++ )
			{
				dind = (struct dindirect *)&indirect2buf[l*4];
				ext2_rd_block( 0, 
						lba + (gbinfo->bs/512) * sind->siblock - 2,
						gbinfo->bs/512, 
						databuf );
			for(index=0;index<4096; index++) *(memloc++)=databuf[index];		
			}
		}	

				
	}
	return SUCCESS;	
}	

/*	This function loads any file whose inode number is given
 *	as argument to any memory location passed as a parameter to the function	*/
int ext2_load_file( UINT inode, UCHAR *load_mem)
{
	int inodesam;
	int i, k; 
	UCHAR groupdescr[1024];
	UCHAR inodebuff[512];
	struct ext2_inode *intab_load[4];

	inodesam=inode_to_lba(inode);

	ext2_rd_block ( 0, current_lba + inodesam, 1, inodebuff) ;

	i=get_index(inode);
	
	intab_load[i]=(struct ext2_inode *)&inodebuff[i*128];

	if ( S_ISREG(intab_load[i]->i_mode) ) 
		ext2_rd_file ( current_lba, intab_load[i], load_mem );

	return(intab_load[i]->i_size);
}

struct file_properties * ext2_root (int lba, int drive, int inode)
{int count;
	current_lba = lba;
	current_drive = drive;
	current_inode = inode;
	ext2_file_prop = ext2_chdir ("/");
	return (ext2_file_prop);
}
