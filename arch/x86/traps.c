#include <include/unix/printk.h>
#include <include/unix/ioport.h>
#include <include/unix/panic.h>
#include <include/unix/sched.h>
#include <include/unix/keyboard.h>
#include <include/unix/signal.h>
#include <include/unix/alarm.h>
#include <include/unix/time.h>

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
	panic("Divide by zero error\n");
}

void debug()
{
	panic("* KERNEL PANIC - DEBUG *");
}

void nmi()
{
	panic("* KERNEL PANIC - NMI *");
}

void int3()
{
	panic("* KERNEL PANIC - INT3 *");
}

void overflow()
{
	panic("* KERNEL PANIC - OVERFLOW *");
}

void bounds()
{
	panic("* KERNEL PANIC - BOUNDS *");
}

void invalid_op()
{
	panic("* KERNEL PANIC - INVALID OP *");
}

void device_not_available()
{
	panic("* KERNEL PANIC - DEVICE NOT AVAILABLE *");
}

void double_fault()
{
	panic("* KERNEL PANIC - Double Fault #DF *");
}

void coprocessor_segment_overrun()
{
	panic("* KERNEL PANIC - Coprocessor Segment Overrun *");
}

volatile long ticks = 0;
volatile long time_ticks = 0;

void clock_handler()
{
	outb(0x20, 0x20);
	ticks += 6;
	time_ticks += 6;

	if (time_ticks >= 100)
	{
		Time.seconds++;
		if (Time.seconds == 60)
		{
			Time.seconds = 0;
			Time.minutes++;
		}
		if (Time.minutes == 60)
		{
			Time.minutes = 0;
			Time.hours++;
		}

		if (Time.hours == 24)
		{
			Time.hours = 0;
			Time.minutes = 0;
			Time.seconds = 0;
		}
		time_ticks = 0;
	}

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

void invalid_tss()
{
	panic("* KERNEL PANIC - Invalid TSS *");
}

void segment_not_present()
{
	panic("* KERNEL PANIC - Segment not present *");
}

void stack_segment()
{
	panic("* KERNEL PANIC - Stack Segment *");
}

void general_protection()
{
	panic("* KERNEL PANIC - General Protection Fault #GP *");
}

void page_fault()
{
	panic("* KERNEL PANIC - Page Fault #PF *");
}

void reserved()
{
	panic("* KERNEL PANIC - Reserved *");
}

void coprocessor_error()
{
	panic("* KERNEL PANIC - Coprocessor error *");
}

unsigned char mouse_cycle = 0;
unsigned char mouse_packet[3];

long mouse_x = 42;
long mouse_y = 15;

void mouse_handler()
{
	unsigned char status = insb(0x64);
	if ((status & 0x01) && status & (1 << 5))
	{
		unsigned char data = insb(0x60);
		
		if (mouse_cycle == 0)
		{
			if (!(data & 0x08))return;
			mouse_packet[0] = data;
			mouse_cycle++;

		} else if (mouse_cycle == 1)
		{
			mouse_packet[1] = data;
			mouse_cycle++;

		} else if (mouse_cycle == 2)
		{
			mouse_packet[2] = data;

			if (mouse_packet[0] & 0x80 || mouse_packet[0] & 0x40)
			{
				mouse_cycle = 0;
				return;
			}

			put_cxy(' ', mouse_x, mouse_y);

			if (mouse_packet[0] & (1 << 0))
			{
				printk("Left BTN");	
			
			} else if (mouse_packet[0] & (1 << 1))
			{
				printk("Right BTN");
			}

			long dx = (signed char)mouse_packet[1];
			long dy = (signed char)mouse_packet[2];

			mouse_x += dx;
			mouse_y -= dy;

			if (mouse_x >= 79)mouse_x = 79;
			if (mouse_y >= 25)mouse_y = 25;
			if (mouse_x <= 0)mouse_x = 0;
			if (mouse_y <= 0)mouse_y = 0;

			put_cxy('/', mouse_x, mouse_y);

			mouse_cycle = 0;
		}
	}
	outb(0xA0, 0x20);
	outb(0x20, 0x20);
}

void keyboard_handler()
{
	keyboard_handler_ext();
	outb(0x20, 0x20);
}

void IdtInstall()
{
	setIdtDescriptor(0, 0x8E, divide_by_zero_error);
	setIdtDescriptor(0x01, 0x8E, debug);
	setIdtDescriptor(0x02, 0x8E, nmi);
	setIdtDescriptor(0x03, 0x8E, int3);
	setIdtDescriptor(0x04, 0x8E, overflow);
	setIdtDescriptor(0x05, 0x8E, bounds);
	setIdtDescriptor(0x06, 0x8E, invalid_op);
	setIdtDescriptor(0x07, 0x8E, device_not_available);
	setIdtDescriptor(0x08, 0x8E, double_fault);
	setIdtDescriptor(0x09, 0x8E, coprocessor_segment_overrun);
	setIdtDescriptor(0x10, 0x8E, invalid_tss);
	setIdtDescriptor(0x11, 0x8E, segment_not_present);
	setIdtDescriptor(0x12, 0x8E, stack_segment);
	setIdtDescriptor(0x13, 0x8E, general_protection);
	setIdtDescriptor(0x14, 0x8E, page_fault);
	setIdtDescriptor(0x15, 0x8E, reserved);
	setIdtDescriptor(0x16, 0x8E, coprocessor_error);
	setIdtDescriptor(0x20, 0x8E, clock_handler);
	setIdtDescriptor(0x21, 0x8E, keyboard_handler);
	setIdtDescriptor(0x2C, 0x8E, mouse_handler);

	idtr.limit = sizeof(idt_table) - 1;
	idtr.base = (long)idt_table;

	__asm__ volatile ("lidt %0" :: "m"(idtr));
}
