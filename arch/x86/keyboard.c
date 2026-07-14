#include <include/unix/printk.h>
#include <include/unix/console.h>
#include <include/unix/ioport.h>
#include <include/unix/signal.h>

#include <include/libc/string.h>

#include <stdbool.h>

char keyboard_buffer[1024];
long pos = 0;

bool ctrl_pressed = false;

void addCharacter(char c)
{
    keyboard_buffer[pos++] = c;
    keyboard_buffer[pos] = '\0';
}

char* monthToString(unsigned char month)
{
    switch(month)
    {
        case 1:
            return "June";
            break;

        case 2:
            return "February";
            break;

        case 3:
            return "March";
            break;

        case 4:
            return "April";
            break;

        case 5:
            return "May";
            break;

        case 6:
            return "June";
            break;

        case 7:
            return "July";
            break;

        case 8:
            return "August";
            break;

        case 9:
            return "September";
            break;

        case 10:
            return "October";
            break;

        case 11:
            return "November";
            break;

        case 12:
            return "December";
            break;

        default:
            return "None";
            break;
    }
}

void keyboard_handler_ext()
{
    unsigned char scancode = insb(0x60);
    if (scancode == 0x1E) {printk("a"); addCharacter('a');}
    if (scancode == 0x30) {printk("b"); addCharacter('b');}
    if (scancode == 0x2E) {
        if (!ctrl_pressed)
        {
            printk("c"); addCharacter('c');
        
        } else
        {
            printk("^C");
            sendsig(current, SIGINT);
        }
    }
    if (scancode == 0x20) {printk("d"); addCharacter('d');}
    if (scancode == 0x12) {printk("e"); addCharacter('e');}
    if (scancode == 0x21) {printk("f"); addCharacter('f');}
    if (scancode == 0x22) {printk("g"); addCharacter('g');}
    if (scancode == 0x23) {printk("h"); addCharacter('h');}
    if (scancode == 0x17) {printk("i"); addCharacter('i');}
    if (scancode == 0x24) {printk("j"); addCharacter('j');}
    if (scancode == 0x25) {printk("k"); addCharacter('k');}
    if (scancode == 0x26) {printk("l"); addCharacter('l');}
    if (scancode == 0x32) {printk("m"); addCharacter('m');}
    if (scancode == 0x31) {printk("n"); addCharacter('n');}
    if (scancode == 0x18) {printk("o"); addCharacter('o');}
    if (scancode == 0x19) {printk("p"); addCharacter('p');}
    if (scancode == 0x10) {printk("q"); addCharacter('q');}
    if (scancode == 0x13) {printk("r"); addCharacter('r');}
    if (scancode == 0x1F) {printk("s"); addCharacter('s');}
    if (scancode == 0x14) {printk("t"); addCharacter('t');}
    if (scancode == 0x16) {printk("u"); addCharacter('u');}
    if (scancode == 0x2F) {printk("v"); addCharacter('v');}
    if (scancode == 0x11) {printk("w"); addCharacter('w');}
    if (scancode == 0x2D) {printk("x"); addCharacter('x');}
    if (scancode == 0x15) {printk("y"); addCharacter('y');}
    if (scancode == 0x2C) {printk("z"); addCharacter('z');}

    if (scancode == 0x39) {printk(" "); addCharacter(' ');}

    if (scancode == 0x1A) {printk("["); addCharacter('[');}
    if (scancode == 0x1B) {printk("]"); addCharacter(']');}
    if (scancode == 0x0D) {printk("="); addCharacter('=');}
    if (scancode == 0x28) {printk("'"); addCharacter('"');}
    if (scancode == 0x29) {printk("`"); addCharacter('`');}
    if (scancode == 0x0C) {printk("-"); addCharacter('-');}
    if (scancode == 0x34) {printk("."); addCharacter('.');}
    if (scancode == 0x33) {printk(","); addCharacter(',');}
    if (scancode == 0x35) {printk("/"); addCharacter('/');}
    if (scancode == 0x27) {printk(";"); addCharacter(';');}
    if (scancode == 0x0F) {printk("     "); addCharacter(' ');}

    if (scancode == 0x1D)
    {
        ctrl_pressed = true;
    }

    if (scancode == 0x9D)
    {
        ctrl_pressed = false;
    }

    if (scancode == 0x1C)
    {
        if (strcmp(keyboard_buffer, "clear") == 0)
        {
            clear();

        } else if (strcmp(keyboard_buffer, "") == 0)
        {
            printk("\n");

        } else if (strcmp(keyboard_buffer, "date") == 0)
        {
            printk("\n");
            char buf[16];
            const char* month = monthToString(bcd_to_bin(cmos_read(0x08)));
            printk(month);
            printk(" ");
            unsigned char day_of_month = bcd_to_bin(cmos_read(0x07));
            itoa(day_of_month, buf, 10);
            printk(buf);
            printk(" ");
            unsigned char hours = bcd_to_bin(cmos_read(0x04));
            itoa(hours, buf, 10);
            printk(buf);
            printk(":");
            unsigned char minutes = bcd_to_bin(cmos_read(0x02));
            itoa(minutes, buf, 10);
            printk(buf);
            printk(":");
            unsigned char seconds = bcd_to_bin(cmos_read(0x00));
            itoa(seconds, buf, 10);
            printk(buf);
            printk("\n");
        }
        
        else
        {
            printk("\n");
            printk(keyboard_buffer);
            printk(": not found\n");
        }

        printk("# ");
        pos = 0;
        keyboard_buffer[pos] = '\0';
    }

}