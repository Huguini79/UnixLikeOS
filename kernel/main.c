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
#include <include/unix/mouse.h>

#include <include/libc/string.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct task_struct* process0;
struct task_struct* process1;
struct task_struct* process2;
struct task_struct* process3;
struct task_struct* process4;

extern char* monthToString(unsigned char month);

int max_y = 0;

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
				video_mem[(i * 80) + j] = 0xF0 << 8 | ' ';
			}
		}


		video_mem[(start_y*80) + end_x-1] = 0x4F << 8 | 'X';

		if (end_y > max_y)
		{
			max_y = end_y;
			y = max_y;
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

void createBar(long start_x, long end_x, long start_y, long end_y)
{
	for (long j = start_x; j < end_x; ++j)
	{
		video_mem[(start_y*80) + j] = 0xF0 << 8 | ' ';
	}
}

void Label(const char* str, int xx, int yy)
{
	size_t len = strlen(str);
	long my_x = xx;
	long my_y = yy;
	for (long i = 0; i < len; ++i)
	{
		video_mem[(my_y * 80) + my_x] = 0x4F << 8 | str[i];
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
	clearWindow(60, 80, 0, 10);
}

void proc0()
{while(1)__asm__ volatile ("sti");}

void proc1()
{}

void proc3()
{signal(SIGINT, alrm_term_handler1);while(1) {createWindow(0, 20, 2, 7); Label("Third process!", 0, 3); for (volatile int i = 0; i < 100000; ++i) {}__asm__ volatile ("sti");}}

void proc2()
{
	
}

void proc4()
{
	// alarm(4);
	// signal(SIGALRM, alrm_term_handler2);
	char buf[16];
	signal(SIGINT, alrm_term_handler2);
		while(1)
		{
				createWindow(60, 80, 2, 7);
				itoa(Time.hours, buf, 10);
				Label(buf, 60, 4);
				Label(":", 62, 4);
				itoa(Time.minutes, buf, 10);
				Label(buf, 63, 4);
				Label(":", 65, 4);
				itoa(Time.seconds, buf, 10);
				Label(buf, 66, 4);
				Label(" ", 68, 4);
				itoa(Time.days, buf, 10);
				Label(buf, 69, 4);
				Label(" ", 71, 4);
				const char* month = monthToString(Time.months);
				Label(month, 72, 4);
				for (volatile int i = 0; i < 100000; ++i) {}
				__asm__ volatile ("sti");
		}
	//}
}

void kernel_init()
{
	time_init();
	mouse_init();
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

	while(1)
	{
		__asm__ volatile ("sti");
	}
}
