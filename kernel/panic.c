#include <include/unix/printk.h>
#include <include/unix/sched.h>

void panic(const char* str)
{
    printk(str);
    while (1) {}
}