#include <include/unix/ioport.h>
#include <include/unix/time.h>

struct time Time;

unsigned char cmos_read(unsigned char value)
{
    outb(0x70, 0x80 | value);
    return insb(0x71);
}

unsigned char bcd_to_bin(unsigned char value)
{
    return (value >> 4) * 10 + (value & 0x0F);
}

void time_init()
{
    Time.seconds = bcd_to_bin(cmos_read(0x00));
    Time.minutes = bcd_to_bin(cmos_read(0x02));
    Time.hours = bcd_to_bin(cmos_read(0x04));
    Time.weekdays = bcd_to_bin(cmos_read(0x06));
    Time.days = bcd_to_bin(cmos_read(0x07));
    Time.months = bcd_to_bin(cmos_read(0x08));
    Time.years = bcd_to_bin(cmos_read(0x09));
    Time.centuries = bcd_to_bin(cmos_read(0x32));
}