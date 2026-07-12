#ifndef CONSOLE_H
#define CONSOLE_H

void ConsoleInstall();
void put_cxy(char c, int x, int y);
void put_c(char c);
void clear();

extern unsigned short* video_mem;
extern long x, y;

#endif