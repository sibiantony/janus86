#ifndef INCLUDE_IDT
#define INCLUDE_IDT

void set_vector(void *handler, unsigned char interrupt, unsigned short control_major);
void InitIDT();

void int_null();
void debug_idt();
void load_gdt();
void init_gdt();

void _int0();
void _int1();
void _int2();
void _int3();
void _int4();
void _int5();
void _int6();
void _int7();
void _int8();
void _int9();
void _int10();
void _int11();
void _int12();
void _int13();
void _int14();
void _int15();
void _int16();

#endif
