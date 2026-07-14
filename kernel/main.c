#include <include/unix/kernel.h>
#include <include/unix/console.h>
#include <include/unix/printk.h>
#include <include/unix/pic.h>
#include <include/unix/traps.h>
#include <include/unix/gdt.h>
#include <include/unix/sched.h>
#include <include/unix/signal.h>
#include <include/unix/alarm.h>
#include <include/unix/time.h>

#include <include/libc/string.h>

#include <stdint.h>
#include <stddef.h>

struct task_struct* process0;
struct task_struct* process1;
struct task_struct* process2;
struct task_struct* process3;
struct task_struct* process4;

void createWindow(long start_x, long end_x, long start_y, long end_y)
{
		for (long j = start_x; j < end_x; ++j)
		{
			video_mem[start_y*80 + j] = 0x1F << 8 | ' ';
		}

		for (long i = start_y+1; i < end_y; ++i)
		{
			for (long j = start_x; j < end_x; ++j)
			{
				video_mem[(i * 80) + j] = 0x4F << 8 | ' ';
			}
		}
}

void clearWindow(long start_x, long end_x, long start_y, long end_y)
{
	for (long j = start_x; j < end_x; ++j)
		{
			video_mem[start_y*80 + j] = 0x3F << 8 | ' ';
		}

		for (long i = start_y+1; i < end_y; ++i)
		{
			for (long j = start_x; j < end_x; ++j)
			{
				video_mem[(i * 80) + j] = 0x3F << 8 | ' ';
			}
		}
}

void Label(const char* str, int xx, int yy)
{
	size_t len = strlen(str);
	long my_x = xx;
	long my_y = yy;
	for (long i = 0; i < len; ++i)
	{
		video_mem[(my_y * 80) + my_x] = 0x2F << 8 | str[i];
		my_x++;
	}
}

void alrm_term_handler1()
{
	current->state = Zombie;
	current->tss.eip = 0;
	clearWindow(0, 30, 0, 16);
}

void alrm_term_handler2()
{
	current->state = Zombie;
	current->tss.eip = 0;
	clearWindow(50, 80, 0, 16);
}

void proc0()
{while(1)__asm__ volatile ("sti");}

void proc1()
{}

void proc3()
{signal(SIGINT, alrm_term_handler1);while(1) {createWindow(0, 30, 0, 16); Label("Third process!", 0, 1); for (volatile int i = 0; i < 100000; ++i) {}__asm__ volatile ("sti");}}

void proc2()
{
	char buf[16];
	printk("Hello World!");
}

void proc4()
{
	// alarm(4);
	// signal(SIGALRM, alrm_term_handler2);
	signal(SIGINT, alrm_term_handler2);
	while (1)
	{
		createWindow(50, 80, 0, 16);
		Label("Fourth process!", 50, 1);
		    for (volatile int i = 0; i < 100000; ++i) {}
		__asm__ volatile ("sti");
	}
}

void kernel_init()
{
	remapPIC();
	GdtInstall();
	IdtInstall();
	ConsoleInstall();
	process0 = initProc(0, proc0);
	process1 = initProc(1, proc1);
	process2 = initProc(2, proc2);
	process3 = initProc(3, proc3);
	process4 = initProc(4, proc4);
	__asm__ volatile ("sti");
}
