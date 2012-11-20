
#ifndef TYPES_H
#define TYPES_H

#define TRUE		1
#define FALSE		0

#define NULL 		(void *)0 

#define LOW_BYTE(x)		(x & 0xFF)
#define HIGH_BYTE(x)		((x >> 8) & 0xFF)
#define EXTENDED_LOW_BYTE(x)	((x >> 16) & 0xFF)
#define EXTENDED_HIGH_BYTE(x)	((x >> 24) & 0xFF)

#define EMPTY


typedef struct anon_sbuiltin_strc {
	char *name;	/**< name of command. */
	int (*fnptr)(int argc, char **argv);	/**< pointer to function. */
	char *help;	/**< message displayed for help. */
} sbuiltin;


typedef unsigned int 		UINT;

typedef unsigned char 		UCHAR;

typedef unsigned short 		USHORT;

typedef unsigned long 		ULONG;

typedef unsigned int		size_t;

/*===================Global variables ===================*/
#define	DIRECTORY_NUM	10
#define PATH_LEN	100

#define SUCCESS 	1
#define FAILURE 	0
#define PROMPT_LEN	8

int current_inode ; 
int current_cluster;
int current_drive ;
UINT current_lba ;
int PART;

#endif

