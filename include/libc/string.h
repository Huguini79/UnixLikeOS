#ifndef LIBC_STRING_H
#define LIBC_STRING_H

#include <stdint.h>
#include <stddef.h>

static inline size_t strlen(const char* str)
{
    size_t i = 0;

    while(str[i] != '\0')
    {
        i++;
    }

    return i;
}

static inline int strcmp(const char* str1, const char* str2)
{
    unsigned char u1, u2;

    while(1)
    {
        u1 = (unsigned char)*str1++;
        u2 = (unsigned char)*str2++;

        if (u1 != u2)
        {
            return u1 - u2;
        }

        if (u2 == '\0')
        {
            return 0;
        }
    }

    return 0;
}

static inline int strncmp(const char* str1, const char* str2, long n)
{
    unsigned char u1, u2;

    for (long i = 0; i < n; ++i)
    {
        u1 = (unsigned char)*str1++;
        u2 = (unsigned char)*str2++;

        if (u1 != u2)
        {
            return u1 - u2;
        }

        if (u2 == '\0')
        {
            return 0;
        }
    }

    return 0;
}

static inline void* memcpy(void* dest, const void* src, size_t size)
{
    char* d = (char*)dest;
    const char* s = (const char*)src;

    for (size_t i = 0; i < size; ++i)
    {
        d[i] = s[i];
    }

    return dest;

}

/* COPIED FROM OSDEV https://wiki.osdev.org/VGA_Text_Mode */
static inline char * itoa( int value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

#endif