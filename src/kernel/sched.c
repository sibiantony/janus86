#include<sched.h>


unsigned int ticks;
unsigned int sys_time;
unsigned int total_ticks;

void clock()
{
    	ticks++;
    	total_ticks++;
    	if(ticks >= 100)
    	{
        	sys_time++;
        	ticks = 0;
    	}
}

void task0()
{
    	while(1) {
	        kprintf("a");
    	}
}

void task1()
{
    	while(1)
    	{
        	kprintf("b");
    	}
}

/* Checks whether proces queue is empty */
int is_empty()
{
    	if(end == front)
        	return 1;
    	return 0;
}

/* Returns the time of current process */
int cur_task_time()
{
    	if(is_empty())
        	return 0;
    	return rrq[front].time;
}

/* Returns the priority of current process */
int cur_task_pri()
{
    	if(is_empty())
        	return 0;
    	return rrq[front].priority;
}

/* Transfers the proces from front to the end */
void front_to_end()
{
     	if(is_empty())
        	return;
     	rrq[end] = rrq[front];
     	end++;
     	if(end >= QUE_SIZE)
         	end = 0;
     	front++;
     	if(front >= QUE_SIZE)
         	front = 0;
}

/* Decreases the time of current process */
void dec_cur_task_time()
{
    	if(is_empty())
        	return;
    	rrq[front].time--;
}

/* Assigns the time of current process to a given value*/
void cur_task_time_equ(int time)
{
    	if(is_empty())
        	return;
    	rrq[front].time = time;
}

/* Returns the time of a process based on the priority */
int get_pri_time(int pri)
{
    	switch(pri)
    	{
        	case 0:
            		return 100;
            		break;
        	case 1:
            		return 10;
            		break;
        	case 2:
            		return 5;
            		break;
        	case 3:
            		return 2;
            		break;
        	case 4:
            		return 1;
            		break;
        	default:
            		return 0;
     	}
}

/* Process Scheduler */
stack_data_t * schedule(stack_data_t *regs)
{
    	kprintf("schedule");
    	cur_task_time_equ(get_pri_time(cur_task_pri()));
    	front_to_end();
    	return rrq[front].stack;
}

/* Task timer */
stack_data_t * task_timer_c(stack_data_t *regs)
{
    	clock();	
    	outportb(0x20, 0x20);
    	return schedule(regs);
}

/* Creates a task with the required memory and values */
void make_task(int pri, char *name, void (*entry)())
{
    	void *stack_mem;
    	stack_data_t *stack;

    	stack_mem = (unsigned int *)kmalloc(STACK_SIZE);
    	stack_mem += STACK_SIZE - sizeof(stack_data_t);
    	stack = stack_mem;

    	stack->gs = DATA_SEG;
    	stack->fs = DATA_SEG;
    	stack->es = DATA_SEG;
    	stack->ds = DATA_SEG;
    	stack->edi = 0;
    	stack->esi = 0;
    	stack->esp = (unsigned int)stack;
    	stack->ebp = stack->esp;
    	stack->ebx = 0;
    	stack->edx = 0;
    	stack->ecx = 0;
    	stack->eax = 0;
    	stack->eip = (unsigned int)entry;
    	stack->cs = KERNEL_CODE_SEG;
    	stack->eflags = 0x00000202;
    
    	strncpy(rrq[end].name, name, 32);
    	rrq[end].stack = stack;
    	rrq[end].ss = KERNEL_STACK_SEG;
    	rrq[end].priority = pri;
    	rrq[end].time = get_pri_time(pri);
    	end++;
}

/* Start process management*/
void start_sys()
{
    	front=0;
    	end=0;    	    	make_task(3, "task0", task0);
    	make_task(3, "task1", task1); 
    	asm("int $33");
    	for(;;);
}
