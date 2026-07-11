#ifndef SCHED_H
#define SCHED_H

enum State
{
    Ready,
    Running,
    Zombie,
    Sleep,
    Swapper,
};

struct tss
{
    long prev_link;
    long esp0;
    long ss0;
    long esp1;
    long ss1;
    long esp2;
    long ss2;
    long cr3;
    long eip;
    long eflags;
    long eax;
    long ecx;
    long edx;
    long ebx;
    long esp;
    long ebp;
    long esi;
    long edi;
    long es;
    long cs;
    long ss;
    long ds;
    long fs;
    long gs;
    long ldtr;
    long iopb;

} __attribute__((packed));

struct task_struct
{
    long pid;
    long ppid;
    long signal;
    long alarm;
    enum State state;
    struct tss tss; /* Hardware multitasking is better than software multitasking! Is more engineering than software multitasking */
};

extern struct task_struct tasks[64];
extern struct task_struct* current;
extern struct task_struct* next;

struct task_struct* initProc(long pid, void* eip);
void schedule();
int exec(struct task_struct* task);

#endif