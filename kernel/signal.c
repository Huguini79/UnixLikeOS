#include <include/unix/signal.h>
#include <include/unix/sched.h>
#include <include/unix/printk.h>
#include <include/libc/string.h>

void sendsig(struct task_struct* task, long signal)
{
    __asm__ volatile ("cli");
    task->signal |= 1U << (signal);
    __asm__ volatile ("sti");
}

void signal(long signal, void* handler)
{
    current->sigactions[signal].signal = signal;
    current->sigactions[signal].handler = handler;
}

void psig()
{
    char buf[16];
    if (current->signal & (1U << SIGTERM))
    {
        printk("* SIGTERM for process with PID=");
        itoa(current->pid, buf, 10);
        printk(buf);
        printk(" *\n");
        current->signal &= ~(1U << SIGTERM);

    } else if (current->signal & (1U << SIGINT))
    {
        printk("* SIGINT for process with PID=");
        itoa(current->pid, buf, 10);
        printk(buf);
        printk(" *\n");
        current->signal &= ~(1U << SIGINT);

    } else if (current->signal & (1U << SIGALRM))
    {
        if (current->sigactions[SIGALRM].handler == 0)
        {
            printk("* SIGALRM for process with PID=");
            itoa(current->pid, buf, 10);
            printk(buf);
            printk(" *\n");
        } else
        {
            void (*action)(void) = (void(*)(void))current->sigactions[SIGALRM].handler;
            action();
        }

        
        current->signal &= ~(1U << SIGALRM);
    }
}