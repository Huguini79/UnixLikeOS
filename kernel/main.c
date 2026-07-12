#include <include/unix/kernel.h>
#include <include/unix/console.h>
#include <include/unix/printk.h>
#include <include/unix/pic.h>
#include <include/unix/traps.h>
#include <include/unix/gdt.h>
#include <include/unix/sched.h>
#include <include/unix/signal.h>
#include <include/unix/alarm.h>

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
				video_mem[(i * 80) + j] = 0xF0 << 8 | ' ';
			}
		}
		
		// video_mem[(start_y * 80) + start_x] = 0x12 << 8 | 'P';
		// video_mem[(start_y * 80) + start_x+1] = 0x12 << 8 | 'R';
		// video_mem[(start_y * 80) + start_x+2] = 0x12 << 8 | 'O';
		// video_mem[(start_y * 80) + start_x+3] = 0x12 << 8 | 'C';
		// video_mem[(start_y * 80) + start_x+4] = 0x12 << 8 | 'E';
		// video_mem[(start_y * 80) + start_x+5] = 0x12 << 8 | 'S';
		// video_mem[(start_y * 80) + start_x+6] = 0x12 << 8 | 'S';
		// video_mem[(start_y * 80) + start_x+7] = 0x12 << 8 | ' ';
		// video_mem[(start_y * 80) + start_x+8] = 0x12 << 8 | '4';
}

void alrm_handler()
{
	printk("ALARM!");
}

void proc0()
{while(1)__asm__ volatile ("sti");}

void proc1()
{}

void proc2()
{alarm(12);while(1) {createWindow(0, 30, 0, 16);__asm__ volatile ("sti");}}

void proc3()
{}

void proc4()
{
	alarm(10);
	signal(SIGALRM, alrm_handler);
	while (1)
	{
		createWindow(50, 80, 0, 16);
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
