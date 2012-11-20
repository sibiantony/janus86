#ifndef __STRING_H
#define __STRING_H

#include "types.h"

void	*memset (void* , int , size_t );
void 	*memcpy (void *, const void *, size_t);
int 	memcmp (const void *, const void *, size_t );
size_t 	strlen (const char* );
size_t 	strnlen (const char *, size_t );
int 	strcmp (const char* , const char* );
int 	strncmp (const char* , const char* , size_t );
char 	*strcat (char *, const char *);
char 	*strcpy (char *, const char *); 
char 	*strncpy (char *, const char *, size_t );
ULONG 	atoi (const char *);
char 	*strchr (const char *, int );
char 	*strrchr (const char *, int );
char 	*strpbrk (const char *, const char *);


#endif
