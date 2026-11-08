#ifndef TRAPS_H
#define TRAPS_H

#include <stdbool.h>

extern bool init_gui;

void IdtInstall();
void wait_for_enter_gui();

#endif
