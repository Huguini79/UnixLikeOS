#include <include/unix/kernel.h>
#include <include/unix/console.h>
#include <include/unix/printk.h>
#include <include/unix/pic.h>
#include <include/unix/traps.h>
#include <include/unix/gdt.h>
#include <include/unix/sched.h>
#include <include/unix/signal.h>
#include <include/unix/alarm.h>

struct task_struct* process1;
struct task_struct* process2;
struct task_struct* process3;

void proc1()
{printk("First process in execution\n");}

void proc2()
{printk("Second process in execution\n");alarm(3);}

void proc3()
{printk("Third process in execution\n");}

void kernel_init()
{
	remapPIC();
	GdtInstall();
	IdtInstall();
	ConsoleInstall();
	process1 = initProc(1, proc1);
	process2 = initProc(2, proc2);
	process3 = initProc(3, proc3);
	while(1)
	{
		__asm__ volatile ("sti");
	}
}
