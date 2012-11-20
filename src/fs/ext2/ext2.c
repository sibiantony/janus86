/*
*	This file contains the code for decoding the inode structure 
*	extracting the files and directories by reading the ext2/3 
*	filesystem structure
*/

#include <ext2.h>

/* Converts an inode number to a LBA address. */

UINT inode_to_lba(UINT inode)
{
	int i,j,small_size;
	i=inode%ext2super->inodes_per_group;
	j=i;   
	inode=inode/ext2super->inodes_per_group;
	small_size=gbinfo->bs/128;
	if(inode==0||inode==1||inode==3||inode==9||inode==27||
		inode==5||inode==25||inode==7||inode==49||inode==81||inode==125||inode==243||inode==343||inode==625)
     {
		inode = (inode*ext2super->blocks_per_group*small_size + (j-1) + 
						grblock->inode_table*small_size)/4;
     }
	 else
	 {
		 inode = (inode*ext2super->blocks_per_group*small_size + (j-1) + 2*small_size)/4;
		 if(gbinfo->bs==1024) inode+=2;
	 }
	 return(inode-2);
}

int get_index(UINT inode)
{
	int i,j;
	i = inode%ext2super->inodes_per_group;	
	j = (i-1)%4;
	return(j);
}

/*
int ext2_rd_root (int drive_num, UINT lba)
{	
	UCHAR groupdescr[1024];
	UCHAR inodebuff[512];
	UCHAR dirbuf[4096];
	int inodesam, i, k, prev_rec_len, j;
	char buf[60];

	ext2_rd_block ( drive_num, lba + gbinfo->gdoffset, 2, groupdescr) ;
	grblock = (struct group *)groupdescr;
	
	inodesam=inode_to_lba(ROOT_INODE);
	ext2_rd_block ( drive_num, lba + inodesam, 1, inodebuff) ;
	i=get_index(ROOT_INODE);

	intab[i]=(struct ext2_inode *)&inodebuff[i*128];
	for (k = 0; k < 12; k++)
	{
		if(intab[i]->i_block[0]!=0)
		{
			prev_rec_len=0;
			ext2_rd_block( drive_num, lba + (gbinfo->bs/512) * intab[i]->i_block[k] - 2,
						gbinfo->bs/512, dirbuf );
			do {
				direntry=(struct ext2_dir_entry_2*)&dirbuf[prev_rec_len];
				for (j=0; j < direntry->name_len; j++)
				{
					kprintf("%c",direntry->name[j]);
				}
				kprintf(" ");
				if ( direntry->rec_len + prev_rec_len >= gbinfo->bs ) goto a;
					prev_rec_len = direntry->rec_len+prev_rec_len;
					
			} while(direntry->rec_len>0);

		}	a:;
	}
			
	kprintf("\n( root done  )\n");
	getch();		 
}
*/
int ext2_rd_inode( int drive_num, UINT lba, UINT inode)
{
	int inodesam;
	int i, k; 
	UCHAR groupdescr[1024];
	UCHAR inodebuff[512];

	ext2_rd_block ( drive_num, lba + gbinfo->gdoffset, 2, groupdescr) ;
	grblock = (struct group *)groupdescr;

	inodesam=inode_to_lba(inode);
	ext2_rd_block ( drive_num, lba + inodesam, 1, inodebuff) ;

	i=get_index(inode);
	intab[i]=(struct ext2_inode *)&inodebuff[i*128];
	if ( S_ISDIR(intab[i]->i_mode) ) ext2_rd_dir ( drive_num, lba, intab[i]);
	/*if ( S_ISREG(intab[i]->i_mode) ) kprintf("file.c");
	if ( S_ISLNK(intab[i]->i_mode) ) kprintf("call link.c");
	if ( S_ISCHR(intab[i]->i_mode) ) kprintf("call chr.c");
	if ( S_ISBLK(intab[i]->i_mode) ) kprintf("call blk.c");
	if ( S_ISFIFO(intab[i]->i_mode) ) kprintf("call fifo.c");
	if ( S_ISSOCK(intab[i]->i_mode) ) kprintf("call sockets.c");*/

}	
