#include <conio.h>
#include <i86.h>

void play_sound(long divisor)
{
    char tmp;

    if (divisor == 0)
    {
        nosound();
        return;
    }

    outp(0x43, 0xb6);
    outp(0x42, (char)divisor);
    outp(0x42, (char)(divisor >> 8));

    tmp = inp(0x61);
    if (tmp != (tmp | 3))
    {
        outp(0x61, tmp | 3);
    }
}


void play_sound_freq(long int freq)
{
    long div;

    if (freq == 0)
    {
        nosound();
        return;
    }

    div = 1193180 / freq;
    play_sound(div);
}

void nosound(void)
{
    char tmp = inp(0x61) & 0xFC;

    outp(0x61, tmp);
}

void pop(void)
{
    char tmp;
    tmp = inp(0x61);
    outp(0x61, tmp | 0x02);
    delay(1);
    outp(0x61, tmp & 0xFD);
}

void play_soundf(long int freq, unsigned int length)
{
    play_sound_freq(freq);
    delay(length);
    nosound();
}
