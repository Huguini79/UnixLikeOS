#include <include/unix/printk.h>

void panic(const char* str)
{
    printk(str);
    while (1) {}
}