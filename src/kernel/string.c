#include <string.h>

void* memset(void* s, int c, size_t n)
{
	unsigned char* p = (unsigned char*) s;

	while (n > 0) {
		*p++ = (unsigned char) c;
	--n;
	}

	return s;
}

void* memcpy(void *dst, const void* src, size_t n)
{
	unsigned char* d = (unsigned char*) dst;
	const unsigned char* s = (const unsigned char*) src;

	while (n > 0) {
		*d++ = *s++;
		--n;
	}

	return dst;
}

int memcmp(const void *s1_, const void *s2_, size_t n)
{
	const signed char *s1 = s1_, *s2 = s2_;

	while (n > 0) {
		int cmp = *s1 - *s2;
		if (cmp != 0)
			return cmp;
		++s1;
		++s2;
	}

	return 0;
}

size_t strlen(const char* s)
{
	size_t len = 0;
	while (*s++ != '\0')
		++len;
	return len;
}

/*
 * This it a GNU extension.
 * It is like strlen(), but it will check at most maxlen
 * characters for the terminating nul character,
 * returning maxlen if it doesn't find a nul.
 * This is very useful for checking the length of untrusted
 * strings (e.g., from user space).
 */
size_t strnlen(const char *s, size_t maxlen)
{
	size_t len = 0;
	while (len < maxlen && *s++ != '\0')
		++len;
	return len;
}


int strcmp(const char* s1, const char* s2)
{
	while (1) {
		int cmp = *s1 - *s2;
		if (cmp != 0 || *s1 == '\0' || *s2 == '\0')
			return cmp;
		++s1;
		++s2;
	}
}

int strncmp(const char* s1, const char* s2, size_t limit)
{
	size_t i = 0;
	while (i < limit) {
		int cmp = *s1 - *s2;
		if (cmp != 0 || *s1 == '\0' || *s2 == '\0')
			return cmp;
		++s1;
		++s2;
		++i;
	}

    /* limit reached and equal */
	return 0;
}

char *strcat(char *s1, const char *s2)
{
	char *t1;

	t1 = s1;
	while (*s1) s1++;
	while(*s2) *s1++ = *s2++;
	*s1 = '\0';

	return t1;
}

char *strcpy(char *dest, const char *src)
{
	char *ret = dest;

	while (*src) {
		*dest++ = *src++;
	}
	*dest = '\0';

	return ret;
}

char *strncpy(char *dest, const char *src, size_t limit)
{
	char *ret = dest;

	while (*src != '\0' && limit > 0) {
		*dest++ = *src++;
		--limit;
	}
	if (limit > 0)
		*dest = '\0';

	return ret;
}

int power(int a, int b)
{
	int pro =1;
	int i;
	for( i = 0; i < b; i++ )
		pro = pro * a;
	return (pro);
}


ULONG atoi(const char *buf) 
{
	ULONG ret = 0;
	int len, i =0 ;
	int val;
	if (*buf  == '0' && (*(buf + 1) == 'x' || *(buf + 1) == 'X'))
	{
		buf += 2;
		len = strlen(buf);
		while (	len > 0 )
		{	
			if(*(buf+(len-1)) >= '0' && *(buf+(len-1)) <= '9') 
					val = *(buf+(len-1)) - '0';
			if(*(buf+(len-1)) >= 'A' && *(buf+(len-1)) <= 'F') 
					val = *(buf+(len-1)) - 'A' + 10 ;
			if(*(buf+(len-1)) >= 'a' && *(buf+(len-1)) <= 'f') 
					val = *(buf+(len-1)) - 'a' + 10 ;
			ret = ret + power(16,i) * val;
			i++; len--;
		}
	}
	else
	while (*buf >= '0' && *buf <= '9') {
		ret *= 10;
		ret += *buf - '0';
		buf++;
	}

	return ret;
}

char *strchr(const char *s, int c)
{
	while (*s != '\0') {
		if (*s == c)
			return (char *) s;
		++s;
	}
	return 0;
}

char *strrchr(const char *s, int c)
{
	size_t len = strlen(s);
	const char *p = s + len;

	while (p > s) {
		--p;
		if (*p == c)
			return (char*) p;
	}
	return 0;
}

char *strpbrk(const char *s, const char *accept)
{
	size_t setLen = strlen(accept);

	while (*s != '\0') {
		size_t i;
		for (i = 0; i < setLen; ++i) {
			if (*s == accept[i])
				return (char *) s;
		}
		++s;
	}

	return 0;
}
