#include <janus86.h>

void k_main ();
/*	Main function of the Janus86 kernel	*/
void k_main()
{	
	kclear_screen ();
/* Initialize memory and start paging*/
	mem_init ();
/* Initialize the fs modules compiled into the kernel*/
	init_fs_modules ();
/* Initialize the 8259 interrupt controller */
/*	Init8259();	*/
/* Initialize the Interrupt descriptor table*/
/*	InitIDT();	*/
/* Calibrate the delay loop */
/*	calibrateDelayLoop(); */

/*Probe the IDE drives present in the system*/
	probe_ide_drives ();
/*Read the disk partitions	*/
	read_disk_partitions ();

/*Enter the shell	*/
	shell_main ();
	while (1);		
}

	
