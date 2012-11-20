#ifndef __SCHED_H
#define __SCHED_H

#include "system.h"
#include "string.h"
#include "gdt.h"
#include "kprintf.h"
#include "kmalloc.h"
#include "x86.h"
#include "delay.h"

#define QUE_SIZE 		128
#define STACK_SIZE    		64*4
#define KERNEL_CODE_SEG  	0x0008
#define KERNEL_STACK_SEG  	0x0010
#define DATA_SEG  		0x0010


int front;
int end;
struct task_data rrq[QUE_SIZE];

void task0();
void task1();
int is_empty();
int cur_task_time();
int cur_task_pri();
void front_to_end();
void dec_cur_task_time();
void cur_task_time_equ(int );
int get_pri_time(int );
stack_data_t * schedule(stack_data_t *);
stack_data_t * task_timer_c(stack_data_t *);
void make_task(int pri, char *name, void (*)());
void start_sys();

extern void task_timer();

#endif
