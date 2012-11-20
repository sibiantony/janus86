#ifndef __SYSTEM_H
#define __SYSTEM_H

#define NULL 0

typedef struct stack_data
{
   	unsigned int gs;
   	unsigned int fs;
   	unsigned int es;
   	unsigned int ds;
   	unsigned int edi;
   	unsigned int esi;
   	unsigned int ebp;
   	unsigned int esp;
   	unsigned int ebx;
   	unsigned int edx;
   	unsigned int ecx;
   	unsigned int eax;
   	unsigned int eip;
   	unsigned int cs;
   	unsigned int eflags;
}stack_data_t;

typedef struct task_data 
{
    	char name[33];
    	struct stack_data *stack;
    	unsigned int ss;
    	unsigned int kstack;
    	unsigned int ustack;
    	unsigned int time;
    	unsigned int priority;
    
}task_data_t;

#endif

