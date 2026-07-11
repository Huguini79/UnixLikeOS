#ifndef GDT_H
#define GDT_H

#include "include/unix/sched.h"

void GdtInstall();
void setTSSDescriptor(struct task_struct* task);

#endif
