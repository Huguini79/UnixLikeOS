#include <include/unix/console.h>
#include <include/libc/string.h>

void printk(const char* str)
{
    size_t len = strlen(str);

    for (int i = 0; i < len; ++i)
    {
        put_c(str[i]);
    }
}