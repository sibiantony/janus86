#ifndef __SHELL_H
#define __SHELL_H

#include "kprintf.h"
#include "string.h"
#include "stdlib.h"
#include "video.h"
#include "keyboard.h"
#include "keydefs.h"
#include "shell_builtin.h"
#include "diskio.h"
#include "types.h"

#define ARGNUM 255
#define CMDLEN 256

#define array_size(x) ((sizeof(x))/(sizeof((x)[0])))
#define ISSPACE(c) ((c) == ' ' || (c) == '\t')

/*	Definition of the Errors	*/
#define MOD_NOT_LOAD		-1
#define FIL_NOT_FOUND		-5
#define NOT_REG_FILE		-6
#define DIR_NOT_FOUND		-7
#define DISK_READ_ERR		-8
#define PAT_LEN_EXCEED		-9
#define CD_ERR			-10
#define BAD_FAT_BOOT_SECTOR	-11
#define BAD_SUPER		-12
#define ARG_LIST_EXCEED		-13
#define PART_NO_SUPPORT		-14
#define NO_SUCH_PART		-15
#define BAD_SIG			-16

void check_commands (char *);
void print_prompt();

void shell_main();
char * getline ();
int shell_parse(char *);
int shell_gen_args(char *, int *, char ***);
int shell_panic(int);
int shell_exec(char *, int , char **);
sbuiltin *shell_cmdtab(int *);
void pto_rmode ();

#endif
