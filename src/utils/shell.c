
#include <shell.h>

//	Specifies all the commands available in the command line interface
static sbuiltin cmdtab[] = {
	{ "help", cmd_help, "help: help [command]" },
	{ "ls" , cmd_ls, "ls: ls [dir]" },
	{ "cd", cmd_cd, "cd: cd [path]" },
	{ "reboot", cmd_reboot, "reboot: reboot" },
	{ "clear", cmd_clear, "clear: clear" },
	{ "ppart", cmd_ppart, "ppart: ppart" },
	{ "more", cmd_more, "more: more filename" },
	{ "load", cmd_load, "load: load [p[partition number]or [-s -f] [sector no. filename]]" },
	{ "ldmod", cmd_ldmod, "ldmod: ldmod [module name] [address]"},
	{ "root", 	cmd_root, 	"root: root p[partition number] " },
	{ "lsdrv", 	cmd_lsdrv, 	"lsdrv: lsdrv"},
	{ "rmdrv", 	cmd_rmdrv, 	"rmdrv: rmdrv [module name]" },
	{ "chload", 	cmd_chload, 	"chload: chload" },
	{ "dump", 	cmd_dump, 	"dump: dump -x or -c [Memory location]" }	
//	{ "send", 	cmd_send, 	"send: send" },
//	{ "receive",    cmd_receive,    "receive: receive"}
	
};

char *strip_leading_whitespace(char *s, int *index)
{

    	while (ISSPACE(*s))
	{	
		++s;
		*index = *index - 1;
	}

    return s;
}

/*	Gets a line of input from the keyboard */
char * getline ()
{
	char ch;
	char *buf, *c;
	int buf_index = 0;
	c=buf;

	while ((ch=getch()) != '\n')
	{
		if (ch == BACKSPACE) 
		{	
			set_cursor_position ( ((cursor_x - 1)<PROMPT_LEN)?PROMPT_LEN:(cursor_x - 1), cursor_y);
			kprintf(" ");
			set_cursor_position ( ((cursor_x - 1)<PROMPT_LEN)?PROMPT_LEN:(cursor_x - 1), cursor_y);
			if (buf_index > 0) { buf--; buf_index--; } 
		}
		else {
			kprintf("%c",ch);
			*buf++ = ch;
			buf_index++;
		}
	}

	cursor_y++;
	cursor_x = 0;			// to account for entry of \n
	c = strip_leading_whitespace (c, &buf_index);
	* (c+buf_index) = '\0';
	return(c);
}

/* Prints the prompt on the screen*/
void print_prompt()
{
	cursor_x = 0;
	set_cursor_position (0, cursor_y);
	kprintf("janus86>");
}

/*	Main function of the Shell, control from janus86 lands up here*/
void shell_main()
{
	char *buffer;int i;
/*================ Global Variable Initialization ===================*/
	current_inode = 2; 
	current_cluster = 2;
	current_drive = 0;
	PART = 1;
	
	while(1)
	{		
		print_prompt();
		buffer = getline();
		shell_parse(buffer);
		strcpy(buffer, "");
	}
}

/*	Parses the line passed as an input to the shell to decipher the commands*/
int shell_parse(char *input)
{	char *dup = input;
	int argc;
	char **argv;

	while ((*dup == ' ') || (*dup == '\t'))
		dup++;
	if (*dup == '\0' || (shell_gen_args(dup, &argc, &argv) != SUCCESS))
		return FAILURE;
	shell_exec(argv[0], argc, argv);
	return SUCCESS;
}

/*	Generates the arguments during parsing of the input to shell	*/
int shell_gen_args(char *c, int *argc, char ***argv)
{	int args;
	static char	*argtable[ARGNUM+1];
	static char sc[CMDLEN+1];
	char *ccp = &sc[0];
	strcpy(ccp, c);
	args = 0;
	while (*ccp) {
		if (args >= ARGNUM) {
			kprintf("Argument List Exceeded\n");
			return FAILURE;
		}
		argtable[args++] = ccp;
		while (*ccp && !(( (*ccp == ' ')&&(*(ccp-1) !='\\') ) || (*ccp == '\t')))
			ccp++;
		while (((*ccp == ' ') || (*ccp == '\t')))
			*ccp++ = '\0';
	}
	argtable[args] = NULL;
	*argc = args;
	*argv = argtable;
	return SUCCESS;
}


int shell_panic(int msg)
{
	switch (msg)
	{
		case 	MOD_NOT_LOAD  :
			kprintf ("Module Not loaded\n"); return(SUCCESS);
			break;
		case 	FIL_NOT_FOUND : 
			kprintf("File not Found\n"); return (SUCCESS);
			break;
		case 	NOT_REG_FILE :
			kprintf("Not a Regular File\n"); return (SUCCESS);
			break;
		case 	DIR_NOT_FOUND :
			kprintf("Directory not found\n"); return (SUCCESS);
			break;
		case 	DISK_READ_ERR :
			kprintf("Disk Read Error\n"); return (SUCCESS);
			break;
		case 	PAT_LEN_EXCEED :
			kprintf("Path Length Exceeded\n");return (SUCCESS);
			break;
		case 	CD_ERR : 
			kprintf("Cannot cd to a file\n"); return (SUCCESS);
			break;
		case 	ARG_LIST_EXCEED : 
			kprintf("Argument List Exceeded\n");return (SUCCESS);
			break;
		case 	PART_NO_SUPPORT	:
			kprintf("Argument List Exceeded\n");return (SUCCESS);
			break;
		case 	NO_SUCH_PART : 
			kprintf ("No such partition: Use ppart to see available partitions\n"); 			return (SUCCESS);
			break;
		case 	BAD_SIG	:
			kprintf ("Bad signature\n");
			return (SUCCESS); 
		default :
			return (FAILURE);
	}
}

/*	Executes the commands and passes control to corresponding functions	*/
int shell_exec(char *name, int argc, char **argv)
{	int x = 0;
	int r;
	while (strcmp(cmdtab[x].name, name) != 0) {
		x++;
		if (x >= array_size(cmdtab)) {
			kprintf("Cannot exec \"%s\".\n", name);
			return FAILURE;
		}
	}
	r = (*cmdtab[x].fnptr)(argc, argv);
	return SUCCESS;
}


sbuiltin *shell_cmdtab(int *el) 
{ 
	*el = array_size(cmdtab); 
	return cmdtab; 
}
