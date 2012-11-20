#include "system.h"

struct task_state {
    	unsigned int eax;
    	unsigned int ebx;
    	unsigned int ecx;
    	unsigned int edx;
    	unsigned int edi;
    	unsigned int esi;
    	unsigned int cs;
    	unsigned int eflags;
    	unsigned int eip;
    	unsigned int esp;
};  
 
struct task_data {
    	unsigned char name[32]
    	unsigned int esp;
    	unsigned int time;
    	unsigned int priority;
    	struct task_state *state;
};

/* Create a task */
unsigned int create_task(void (*function)(), unsigned int priority, unsigned char *name)
{
   	struct task_data *new_task = (struct task_data *)malloc(sizeof(struct task_data));
   	strcpy(new_task[0].name, name);
   	new_task[0].esp = getesp();
   	new_task[0].time = 10;
   	new_task[0].task_priority = priority;
   	new_task[0].state = (struct task_state *)malloc(sizeof(struct task_state));
   	new_task[0].state->eax = 0;
   	new_task[0].state->ebx = 0;
   	new_task[0].state->ecx = 0;
   	new_task[0].state->edx = 0;
   	new_task[0].state->edi = 0;
   	new_task[0].state->esi = 0;
   	new_task[0].state->cs = 0x8;
   	new_task[0].state->eflags = 0x202;
   	new_task[0].state->eip = (unsigned)function;
   	new_task[0].state->esp = (unsigned int)new_task[0].state;
   	return (unsigned int) new_task;
}

