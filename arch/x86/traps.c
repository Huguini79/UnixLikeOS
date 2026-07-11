#include <include/unix/printk.h>
#include <include/unix/ioport.h>
#include <include/unix/panic.h>
#include <include/unix/sched.h>
#include <include/unix/keyboard.h>
#include <include/unix/signal.h>
#include <include/unix/alarm.h>

struct Idt
{
	unsigned short isr_low;
	unsigned short selector;
	unsigned char reserved;
	unsigned char type_attributes;
	unsigned short isr_high;
	
} __attribute__((packed));

struct IdtRegister
{
	unsigned short limit;
	long base;
	
} __attribute__((packed));

/*
struct interrupt_frame
{
	long edi;
	long esi;
	long ebp;
	long reserved;
	long ebx;
	long edx;
	long ecx;
	long eax;
	long eip;
	long cs;
	long eflags;
	long esp;
	long ss;
	
} __attribute__((packed));
*/

struct IdtRegister idtr;

struct Idt idt_table[256] __attribute__((aligned(8)));

/*
extern void divide_by_zero_error_stub();
extern void double_fault_stub();
extern void clock_handler_stub();
extern void keyboard_handler_stub();

void divide_by_zero_error();
void double_fault();
void clock_handler();
void keyboard_handler();
*/

void setIdtDescriptor(long vector, unsigned char type_attributes, void* isr)
{
	struct Idt* Idt = &idt_table[vector];
	Idt->isr_low = (long)isr & 0xFFFF;
	Idt->selector = 0x08;
	Idt->reserved = 0;
	Idt->type_attributes = type_attributes;
	Idt->isr_high = (long)isr >> 16 & 0xFFFF;
}

void divide_by_zero_error()
{
	printk("Divide by zero error\n");
}

void double_fault()
{
	panic("* KERNEL PANIC - Double Fault #DF *");
}

volatile long ticks = 0;

void clock_handler()
{
	outb(0x20, 0x20);
	ticks += 7;
	for (long i = 0; i < 64; ++i)
	{
		if (callouts[i].ticks != 0)
		{
			if (callouts[i].ticks <= ticks)
			{
				callouts[i].ticks = 0;
				sendsig(callouts[i].task, SIGALRM);
			}
		}
	}
	schedule();
}

void keyboard_handler()
{
	keyboard_handler_ext();
	outb(0x20, 0x20);
}

void IdtInstall()
{
	setIdtDescriptor(0, 0x8E, divide_by_zero_error);
	setIdtDescriptor(0x08, 0x8E, double_fault);
	setIdtDescriptor(0x20, 0x8E, clock_handler);
	setIdtDescriptor(0x21, 0x8E, keyboard_handler);

	idtr.limit = sizeof(idt_table) - 1;
	idtr.base = (long)idt_table;

	__asm__ volatile ("lidt %0" :: "m"(idtr));
}
