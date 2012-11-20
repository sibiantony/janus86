#include <elf.h>

void elf_print_error (char *message)
{
	kprintf ("%s",message);
	/*exit(0); */
}

void debug_elf_header (Elf32_Ehdr *elf_hdr)
{
	kprintf ("e_ident : %X", 			elf_hdr->e_ident[EI_MAG0] );
	kprintf (" e_ident : %c%c%c\n", 			elf_hdr->e_ident[EI_MAG1], 
		elf_hdr->e_ident[EI_MAG2], elf_hdr->e_ident[EI_MAG3] );
	kprintf ("e_ident : \tFile Class \t%d\n", 	elf_hdr->e_ident[EI_CLASS] );
	kprintf ("e_ident : \tData Encoding \t%d\n", 	elf_hdr->e_ident[EI_DATA] );
	kprintf ("e_ident : \tFile Version \t%d\n", 	elf_hdr->e_ident[EI_VERSION] );
	
	kprintf ("e_type : \tObject File type \t%d\n", 	elf_hdr->e_type);
	kprintf ("e_machine : \tMachine arch. \t%d\n", 	elf_hdr->e_machine);
	kprintf ("e_version : \tObject file version \t%d\n", elf_hdr->e_version);	
	kprintf ("e_entry : \tVirtual address to which system first transfers control \t%d\n",
							elf_hdr->e_entry);
	kprintf ("e_phoff : \tProgram header table's file offset(bytes) \t%d\n",
							elf_hdr->e_phoff);
	kprintf ("e_shoff : \tSection header table's file offset(bytes) \t%d\n",
							elf_hdr->e_shoff);
	kprintf ("e_flags : \tProcessor specific flags \t%d\n", elf_hdr->e_flags);
	kprintf ("e_ehsize : \tELF header's size \t%d\n", elf_hdr->e_ehsize);
	kprintf ("e_phentsize : \tSize of one entry of PH  \t%d\n", elf_hdr->e_phentsize);
	kprintf ("e_phnum : \tNumber of entries in PHt \t%d\n", elf_hdr->e_phnum);
	kprintf ("e_shentsize : \tsize of one entry of SH  \t%d\n", elf_hdr->e_shentsize);
	kprintf ("e_shnum : \tNumber of entries in SHt \t%d\n", elf_hdr->e_shnum);
	kprintf ("e_shstrndx : \tSHt index of entry associated with section name string table \
			%d\n", 				elf_hdr->e_shstrndx);
}


void debug_section_header (Elf32_Shdr *elf_shdr)
{
	/*kprintf("\nsh_name : Section name\t%d\n", 	elf_shdr->sh_name);
	kprintf("sh_type : Section type\t%X\n", 		elf_shdr->sh_type);
	kprintf("sh_flags : Section attributes\t%d\n", 	elf_shdr->sh_flags);
	kprintf("sh_addr : The addr of the 1st byte of section if it appears in the memory image of a process\t0x%X\n", 					elf_shdr->sh_addr);
	kprintf("sh_offset : Section offset from the beginning of file\t0x%X\n", 
							elf_shdr->sh_offset);
	kprintf("sh_size : Section's size\t%d\n", 	elf_shdr->sh_size);
	kprintf("sh_link : SHt index link\t%d\n", 	elf_shdr->sh_link);
	kprintf("sh_info : Extra information\t%d\n", 	elf_shdr->sh_info);
	kprintf("sh_addralign : Alignm't constraints\t%d\n", 
							elf_shdr->sh_addralign);
	kprintf("sh_entsize : Size of fixed-size entries in a section\t%d\n", 
							elf_shdr->sh_entsize);*/
	kprintf ("%d\t", elf_shdr->sh_name);
	kprintf ("0x%-11X\t", elf_shdr->sh_type);
	kprintf ("%-5d\t", elf_shdr->sh_flags);
	kprintf ("0x%-11X\t", elf_shdr->sh_addr);
	kprintf ("0x%-8X\t", elf_shdr->sh_offset);
	kprintf ("%-7d\t", elf_shdr->sh_size);
	kprintf ("%-7d\t", elf_shdr->sh_link);
	kprintf ("%-7d\t", elf_shdr->sh_info);
	kprintf ("%-7d\t", elf_shdr->sh_addralign);
	kprintf ("%-7d\n", elf_shdr->sh_entsize);

}

void debug_symboltable_entry (Elf32_Sym *elf_sym, 
				unsigned char *elf_strtab)
{
	kprintf ("%25s", &elf_strtab[elf_sym->st_name]);
	kprintf ("\t%10d", elf_sym->st_value);
	kprintf ("\t\t%d", elf_sym->st_size);
	kprintf ("\t%d", elf_sym->st_info);
	kprintf ("\t%d", elf_sym->st_other);
	kprintf ("\t0x%X\n", elf_sym->st_shndx);
}

void debug_program_header (Elf32_Phdr *elf_phdr)
{
	kprintf (" \n%10d\t", elf_phdr->p_type);
	kprintf (" %10d\t", elf_phdr->p_offset);
	kprintf (" %10d\t", elf_phdr->p_vaddr);
	kprintf (" %10d\t", elf_phdr->p_paddr);
	kprintf (" %5d\t", elf_phdr->p_filesz);
	kprintf (" %5d\t", elf_phdr->p_memsz);
	kprintf (" %5d\t", elf_phdr->p_flags);
	kprintf (" %5d\t", elf_phdr->p_align);
}

void debug_string_tables (unsigned char *elf_strtab)
{
	int i = 0;
	kprintf ("\n");
	for (i = 0; i < strtab_size; i++)
		kprintf ("%c", elf_strtab[i]);
	kprintf ("\n\n");
}

Elf32_Phdr **read_program_headers (Elf32_Ehdr *elf_hdr,	UCHAR *elf_ptr)
{
	Elf32_Phdr **elf_phdr;
	unsigned char buf [ELF_PH_SIZE];
	int i = 0;
	
	/* Allocate memory for each entry in the program header table */
	elf_phdr = (Elf32_Phdr **)kmalloc (elf_hdr->e_phnum);
	
	for (i = 0 ; i < elf_hdr->e_phnum; i++)
	{	
		/* We are creating a program header table in memory 	*/
		elf_phdr[i] = (Elf32_Phdr *)kmalloc (ELF_PH_SIZE);
		elf_phdr[i] = (Elf32_Phdr *)&elf_ptr[elf_hdr->e_phoff +
				 i * ELF_PH_SIZE];
		debug_program_header (elf_phdr[i]);
	}
	
	return (elf_phdr);
}
		
Elf32_Sym **read_symbol_table (Elf32_Ehdr *elf_hdr, Elf32_Shdr **elf_shdr, 
				unsigned char *elf_strtab, 
				UCHAR *elf_ptr)
{
	Elf32_Sym **elf_sym;
	int symtab_size = 0;
	int tab_size = 0;
	int tab_entry = 0;
	int i;
	
	for (i = 0; i < elf_hdr->e_shnum; i++)
	{	
		if (elf_shdr[i]->sh_type == SHT_SYMTAB ||
			 elf_shdr[i]->sh_type == SHT_DYNSYM) 
			symtab_size += elf_shdr[i]->sh_size;				
	}
	
	symtab_num = (symtab_size / (ELF_STENTRY_SIZE) ) + 1;
	elf_sym = (Elf32_Sym **)kmalloc (symtab_num);
	
	for (i = 0, symtab_num = 0; i < elf_hdr->e_shnum; i++)
	{	
		if (elf_shdr[i]->sh_type == SHT_SYMTAB ||
			 elf_shdr[i]->sh_type == SHT_DYNSYM) 
		/* if the section is of type symtab or dynsym add it to the symbol table*/
		{	
			tab_size = elf_shdr[i]->sh_size;
			
			for (tab_entry = 0; tab_size; tab_entry++, symtab_num ++)
			{
				/* Allocate memory for each symbol table entry	*/
				elf_sym[symtab_num] = (Elf32_Sym *)kmalloc (ELF_STENTRY_SIZE);

				elf_sym[symtab_num] = 
					(Elf32_Sym *)&elf_ptr[elf_shdr[i]->sh_offset +
						tab_entry * ELF_STENTRY_SIZE];
				debug_symboltable_entry (elf_sym[symtab_num], elf_strtab);
				tab_size -= ELF_STENTRY_SIZE;			
			}
		}	
	}
	return (elf_sym);
}

unsigned char *read_string_tables (Elf32_Ehdr *elf_hdr, Elf32_Shdr **elf_shdr,
				UCHAR *elf_ptr)
{
	int i;
	int strtab_ptr = 0; 
	unsigned char *elf_strtab;
	
	/* compute the total size for string tab.*/
	for (i = 0 ; i < elf_hdr->e_shnum; i++)
	{
		if (elf_shdr[i]->sh_type == SHT_STRTAB)	
			strtab_size += elf_shdr[i]->sh_size;
	}
	/* Allocate memory for our string table.	*/
	elf_strtab = (unsigned char *)kmalloc (strtab_size);
	
	for (i = 0; i < elf_hdr->e_shnum; i++)
	{	
		if (elf_shdr[i]->sh_type == SHT_STRTAB) 
		/* if the section is a string table add it to our string table*/
		{
		/* Well, here we need a memcpy(), so till then.. */
			/*elf_strtab[strtab_ptr] = 
				(UCHAR *)&elf_ptr [ elf_shdr[i]->sh_offset +
				i * elf_shdr[i]->sh_size]; */
			strtab_ptr += elf_shdr[i]->sh_size;
		}
		
	}
	strtab_size = strtab_ptr;
	
	debug_string_tables (elf_strtab);
	return (elf_strtab);
}

Elf32_Shdr **read_section_headers (Elf32_Ehdr *elf_hdr, UCHAR *elf_ptr)
{
	int i ;
	unsigned char buf[ELF_SH_SIZE];
	Elf32_Shdr **elf_shdr;
	
	/* Allocate memory for each entry in the section header table */
	elf_shdr = (Elf32_Shdr **)kmalloc (elf_hdr->e_shnum);
	//fseek (fptr, elf_hdr->e_shoff, SEEK_SET);
	
	kprintf ("\nsh_name sh_type    sh_flags \tsh_addr \tsh_offset \tsh_size sh_link sh_info sh_addralign  sh_entsize\n");

	for (i = 0 ; i < elf_hdr->e_shnum; i++)
	{	
		/* We are creating a section header table in memory 	*/
		elf_shdr[i] = (Elf32_Shdr *)kmalloc (ELF_SH_SIZE);
		elf_shdr[i] = (Elf32_Shdr *)&elf_ptr[elf_hdr->e_shoff +
					i * ELF_SH_SIZE ];
		debug_section_header (elf_shdr[i]);
	}
	
	return (elf_shdr);
}

void check_elf_header (Elf32_Ehdr *elf_hdr)
{
	/* ELF checks
 	*/
    	if (elf_hdr->e_ident[0] != ELFMAG0 ||
		elf_hdr->e_ident[1] != ELFMAG1 ||
		elf_hdr->e_ident[2] != ELFMAG2 ||
		elf_hdr->e_ident[3] != ELFMAG3 )
         		elf_print_error ("Invalid File Format\n");
         
	/* We only work on intel ...
	*/
	if (elf_hdr->e_type != ET_EXEC )
		elf_print_error ("Unsopprted execution format \n") ;
	if (elf_hdr->e_machine != EM_386 )
		elf_print_error ("Unsupported e_machine format \n") ;
	if (elf_hdr->e_version != EV_CURRENT )
		elf_print_error ("Invalid e_version format \n") ;

}

Elf32_Ehdr *read_elf_header (UCHAR *elf_ptr)
{
	unsigned char buf[ELF_HEADER_SIZE];
	Elf32_Ehdr *elf_hdr;
	
	/* Read the elf header		*/
	elf_hdr = (Elf32_Ehdr *)kmalloc (ELF_HEADER_SIZE);
	elf_hdr = (Elf32_Ehdr *)&elf_ptr [0];
	debug_elf_header (elf_hdr);
	
	return (elf_hdr);
}

int elf_load(UCHAR *elf_ptr)
{
	Elf32_Ehdr *elf_hdr; 
	Elf32_Shdr **elf_shdr;
	Elf32_Sym **elf_sym;
	Elf32_Phdr **elf_phdr;
	unsigned char *elf_strtab;
	
	int i = 0;
	
	/* Read the elf header		*/
	elf_hdr = read_elf_header (elf_ptr);
	check_elf_header (elf_hdr);
	
	/* Now start reading the section headers	*/
	elf_shdr = read_section_headers (elf_hdr, elf_ptr);
	/* Then the string tables...	*/
	elf_strtab = read_string_tables (elf_hdr, elf_shdr, elf_ptr);
	
	/* ...And the symbol table	*/
	elf_sym = read_symbol_table (elf_hdr, elf_shdr, elf_strtab, elf_ptr);
	
	/* Read the program headers 	*/
	elf_phdr = read_program_headers (elf_hdr, elf_ptr);
	
		
	/* Keep everything clean. Free all the allocated mem */
	kfree (elf_hdr);
	kfree (elf_shdr);
	kfree (elf_sym);
	kfree (elf_phdr);
	kfree (elf_phdr);
	kfree (elf_strtab);
	
	return 0;
}
