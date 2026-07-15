#include <include/unix/sched.h>
#include <include/unix/gdt.h>
#include <include/libc/string.h>

#define CODE_SELECTOR 0x08
#define DATA_SELECTOR 0x10

struct task_struct tasks[64] = {0};
struct task_struct ready[64] = {0};
struct task_struct* current = &tasks[0];
struct task_struct* next = &tasks[0];

void blank()
{
    while (1)__asm__ volatile ("sti");
}

struct task_struct* initProc(long pid, void* eip)
{
    struct task_struct* newTask = &tasks[pid];
    newTask->pid = pid;
    newTask->signal = 0;
    newTask->alarm = 0;
    newTask->state = Ready;
    newTask->tss.esp0 = 0x600000 + pid * 4096;
    newTask->tss.ss0 = 0x10;
    newTask->tss.eip = (long)eip;
    newTask->tss.eflags = 0x202;
    newTask->tss.eax = 0;
    newTask->tss.ecx = 0;
    newTask->tss.edx = 0;
    newTask->tss.ebx = 0;
    long* stack = (long*)0x3FF00 + pid * 8192;
    *stack = (long)blank;
    newTask->tss.esp = (long)stack;
    newTask->tss.ebp = 0;
    newTask->tss.esi = 0;
    newTask->tss.edi = 0;
    newTask->tss.es = DATA_SELECTOR;
    newTask->tss.cs = CODE_SELECTOR;
    newTask->tss.ss = DATA_SELECTOR;
    newTask->tss.ds = DATA_SELECTOR;
    newTask->tss.fs = DATA_SELECTOR;
    newTask->tss.gs = DATA_SELECTOR;
    newTask->tss.iopb = 0x80000000;

    setTSSDescriptor(newTask);

    return newTask;
}

void schedule()
{
    if (tasks[current->pid+1].state != Zombie)
    {
        if (tasks[current->pid+1].tss.eip != 0)
        {
            next = &tasks[current->pid+1];

        } else
        {
            for (long i = 0; i < 64; ++i)
            {
                if (tasks[i].tss.eip != 0)
                {
                    next = &tasks[i];
                    break;
                }
            }
        }

        current->state = Ready;
        current = next;
        current->state = Running;

        if (current->signal != 0)
        {
            psig();
        }

        if (current->tss.eip != 0)
        {
            exec(current);
        }
    
    } else
    {
        long cur_pid = -1;
        for (long i = 1; i < 64; ++i)
        {
            if (tasks[(current->pid+i) % 64].tss.eip != 0 && tasks[(current->pid+i) % 64].state != Zombie)
            {
                cur_pid = tasks[(current->pid+i) % 64].pid;
                current = &tasks[cur_pid];
                break;
            }
        }

        if (cur_pid == -1)
            current = &tasks[0];

        exec(current);
    }
}

int exec(struct task_struct* task)
{
    unsigned short selector = (task->pid+3) * 8;

    volatile struct
    {
        long offset;
        unsigned short selector;

    } __attribute__((packed)) _tmp;

    _tmp.offset = 0;
    _tmp.selector = selector;

    __asm__ volatile ("ljmp %0" :: "m"(_tmp));
    
}