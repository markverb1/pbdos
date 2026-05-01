#ifndef _BEEPER_H
#define _BEEPER_H

#include "notes.h"

void play_sound_freq(long int freq);
void play_sound(long divisor);
void nosound(void);
void play_soundf(long int freq, unsigned int length);
void pop(void);

#endif
