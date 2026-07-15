#include <include/unix/ioport.h>

void mouse_wait(unsigned char a_type)
{
    long timeout = 100000;

    if (!a_type)
    {
        while (--timeout)
        {
            if ((insb(0x64) & 0x01) == 1)return;
        }

    } else
    {
        while (--timeout)
        {
            if (!((insb(0x64) & 0x02)))return;
        }
    }
}

unsigned char mouse_read()
{
    mouse_wait(0);
    return insb(0x60);
}

void mouse_init()
{
    unsigned char status;
    outb(0x64, 0xA8);
    outb(0x64, 0x20);
    status = insb(0x60) | (1 << 1) | (0 << 5);
    outb(0x64, 0x60);
    outb(0x60, status);
    outb(0x64, 0xD4);
    outb(0x60, 0xF6);
    mouse_read();
    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
    mouse_read();
}