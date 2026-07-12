#include <include/libc/string.h>
#include <include/unix/ioport.h>
#include <include/unix/console.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEM 0xB8000

#define DEFAULT_FONT 0x3F

unsigned short* video_mem;
long x, y = 0;

void ConsoleInstall()
{
    video_mem = (unsigned short*)VGA_MEM;
    clear();
}

void put_cxy(char c, int x, int y)
{
    video_mem[(y * VGA_WIDTH) + x] = DEFAULT_FONT << 8 | c;
}

void enable_cursor(unsigned char cursor_start, unsigned char cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (insb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (insb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int x, int y)
{
	unsigned short pos = (y+1) * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (unsigned char) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void put_c(char c)
{
    if (x >= VGA_WIDTH)
    {
        x = 0;
        y++;
        update_cursor(x, y);
    }

    if (y >= VGA_HEIGHT)
    {
        clear();
    }

    if (c == '\n')
    {
        x = 0;
        y++;
        update_cursor(x, y);

    } else
    {
        video_mem[(y * VGA_WIDTH) + x] = DEFAULT_FONT << 8 | c;
        x++;
        update_cursor(x, y);
    }
}

void clear()
{
    disable_cursor();
    x = 0;
    y = 0;

    for (int i = 0; i < VGA_HEIGHT; ++i)
    {
        for (int j = 0; j < VGA_WIDTH; ++j)
        {
            put_c(' ');
        }
    }

    x = 0;
    y = 0;

    enable_cursor(0, 1);
    update_cursor(0, 0);

}
