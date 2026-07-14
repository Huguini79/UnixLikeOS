#include <include/unix/ioport.h>

unsigned char cmos_read(unsigned char value)
{
    outb(0x70, 0x80 | value);
    return insb(0x71);
}

unsigned char bcd_to_bin(unsigned char value)
{
    return (value >> 4) * 10 + (value & 0x0F);
}