#ifndef ALARM_H
#define ALARM_H

#include <include/unix/sched.h>

struct callout
{
    long ticks;
    struct task_struct* task;
};

extern struct callout callouts[64];

void alarm(long s);
void sleep(long s);

#endif