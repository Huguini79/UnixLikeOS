#ifndef TIME_H
#define TIME_H

struct time
{
    long seconds;
    long minutes;
    long hours;
    long weekdays;
    long days;
    long months;
    long years;
    long centuries;
};

extern struct time Time;

unsigned char cmos_read(unsigned char value);
unsigned char bcd_to_bin(unsigned char value);
void time_init();

#endif