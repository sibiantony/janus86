#ifndef	__LIB_KERN__
#define __LIB_KERN__

/*
 * Some common stuff normally seen as lib routines,
 * but done in the kernel. Also some common kern
 * stuff that isn't any sublayer .. a la 'library' :-)
 */

#include<stdarg.h>
#include<types.h>

extern int sprintf(char *,const char *,...);
extern int snprintf(char *,size_t,const char *,...);
extern int vsnprintf(char *,size_t,const char *,va_list);

extern int sscanf(const char *,const char *,...);

extern long strtol(char *,int base);
extern unsigned long strtoul(char *,int base);

#endif
