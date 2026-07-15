#ifndef SCHED_H
#define SCHED_H

#include <include/unix/signal.h>

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

struct sigaction
{
    long signal;
    void* handler;
};

struct task_struct
{
    long pid;
    long ppid;
    long signal;
    long alarm;
    enum State state;
    struct sigaction sigactions[32];
    struct tss tss;
};

extern struct task_struct tasks[64];
extern struct task_struct ready[64];
extern struct task_struct* current;
extern struct task_struct* next;

struct task_struct* initProc(long pid, void* eip);
void schedule();
int exec(struct task_struct* task);

#endif