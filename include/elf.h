
#ifndef __ELF_H
#define __ELF_H

#include "kprintf.h"
#include "kmalloc.h"

#define Elf32_Addr 	unsigned long
#define Elf32_Half	unsigned short
#define Elf32_Off	unsigned long
#define Elf32_Sword	long
#define Elf32_Word	unsigned long

#define EI_NIDENT     	16

typedef struct {
        unsigned char e_ident[EI_NIDENT];
        Elf32_Half    e_type;
        Elf32_Half    e_machine;
        Elf32_Word    e_version;
        Elf32_Addr    e_entry;
        Elf32_Off     e_phoff;
        Elf32_Off     e_shoff;
        Elf32_Word    e_flags;
        Elf32_Half    e_ehsize;
        Elf32_Half    e_phentsize;
        Elf32_Half    e_phnum;
        Elf32_Half    e_shentsize;
        Elf32_Half    e_shnum;
        Elf32_Half    e_shstrndx;
} Elf32_Ehdr;

typedef struct {
        Elf32_Word sh_name;
        Elf32_Word sh_type;
        Elf32_Word sh_flags;
        Elf32_Addr sh_addr;
        Elf32_Off  sh_offset;
        Elf32_Word sh_size;
        Elf32_Word sh_link;
        Elf32_Word sh_info;
        Elf32_Word sh_addralign;
        Elf32_Word sh_entsize;
} Elf32_Shdr;

typedef struct {
        Elf32_Word    st_name;
        Elf32_Addr    st_value;
        Elf32_Word    st_size;
        unsigned char st_info;
        unsigned char st_other;
        Elf32_Half    st_shndx;
} Elf32_Sym;

/* ELF program header 	*/
typedef struct {
	Elf32_Word p_type;
	Elf32_Off  p_offset;
	Elf32_Addr p_vaddr;
	Elf32_Addr p_paddr;
	Elf32_Word p_filesz;
	Elf32_Word p_memsz;
	Elf32_Word p_flags;
	Elf32_Word p_align;
} Elf32_Phdr;

/* e_ident[] identification indexes */
#define EI_MAG0			0
#define EI_MAG1			1
#define EI_MAG2			2
#define EI_MAG3			3
#define EI_CLASS		4
#define EI_DATA			5
#define EI_VERSION		6
#define EI_PAD			7
#define EI_NIDENT 		16

/*	ELF Magic number 	*/
#define ELFMAG0 		0x7F
#define ELFMAG1 		'E'
#define ELFMAG2 		'L'
#define ELFMAG3 		'F'

/*	Object file Types 	*/
#define ET_NONE			0
#define ET_REL			1
#define ET_EXEC			2
#define ET_DYN			3
#define ET_CORE			4
#define ET_LOPROC 		0xFF00
#define ET_HIPROC 		0xFFFF

/*	Machine Type 		*/
#define EM_NONE			0
#define EM_386			3	/* Intel 386	*/

/* 	Object File version	*/
#define EV_NONE			0
#define EV_CURRENT 		1

/* Some section header table indexes are reserved; an object ﬁle
 will not have sections for these special indexes.	*/
#define SHN_UNDEF		0
#define SHN_LORESERVE		0xFF00
#define SHN_LOPROC		0xFF00
#define SHN_HIPROC		0xFF1F
#define SHN_ABS			0xFFF1
#define SHN_COMMON		0xFFF2
#define SHN_HIRESERVE		0xFFFF

/* Different section types       */
#define SHT_NULL		0
#define SHT_PROGBITS		1
#define SHT_SYMTAB		2
#define SHT_STRTAB		3
#define SHT_RELA		4
#define SHT_HASH		5
#define SHT_DYNAMIC		6
#define SHT_NOTE		7
#define SHT_NOBITS		8
#define SHT_REL			9
#define SHT_SHLIB		10
#define SHT_DYNSYM		11
#define SHT_LOPROC		0x70000000
#define SHT_HIPROC		0x7fffffff
#define SHT_LOUSER		0x80000000
#define SHT_HIUSER		0xffffffff

/* A symbol table's index 0 entry is undefined	*/
#define STN_UNDEF		0

/* Interpret the 'st_info' field of the elf symbol table	*/
#define ELF32_ST_BIND(x)   	((x) >> 4)
#define ELF32_ST_TYPE(x)   	((x) & 0xf)
#define ELF32_ST_INFO(b,t) 	(((b) << 4) + ((t) & 0xf))

int strtab_size = 0;	/* The array size for the memory mapped string table */
int symtab_num = 0;	/* Number of entries in symbol table , symbols are from both the 
			section types SHT_SYMTAB and SHT_DYNSYM */

#define ELF_HEADER_SIZE		sizeof (Elf32_Ehdr)
#define ELF_SH_SIZE		sizeof (Elf32_Shdr)
#define ELF_STENTRY_SIZE	sizeof (Elf32_Sym)
#define ELF_PH_SIZE		sizeof (Elf32_Phdr)

#endif
