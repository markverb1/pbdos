#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <graph.h>
#include <bios.h>
#include <i86.h>
#include <string.h>
#include <time.h>
#include "beep.h"
#include "pbutil.h"

// Colors
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define WHITE 7
#define GRAY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define BRIGHTWHITE 15
#define LIGHTYELLOW 14

#define BLUSEG 0
#define X2SEG 1
#define X3SEG 2
#define YELSEG 3
#define REDSEG 4
#define MAGSEG 5
#define GRYSEG 6
#define GRNSEG 7
#define UNKSEG 8

// Randomize by writing ?
struct Segment
{
  unsigned int color;
  char *label;
  char *inbar;
};

static struct Segment segmenttypes[9] = {
    {LIGHTBLUE, "BYTE", "B"},
    {CYAN, "2 BYTES", "0"},
    {CYAN, "3 BYTES", "0"},
    {YELLOW, "CORRUPTED", "?"}, // Random char
    {RED, "ERROR", "?"},
    {MAGENTA, "DEALLOCATE", "0"},
    {GRAY, "NULL", "0"},
    {GREEN, "ALL BYTES", "B"},
    {BRIGHTWHITE, "UNKNOWN", "0"}};

const char hexchars[] = "0123456789ABCDEF";
char random_hex(bool allow_zero)
{
  if (allow_zero)
    return hexchars[randbetween(0, 15)];
  else
    return hexchars[randbetween(1, 15)];
}

unsigned int chooseseg(bool return_unkseg)
{
  unsigned int percent = return_unkseg ? randbetween(1, 95) : randbetween(1, 85);
  if (percent < 20)
    return BLUSEG;
  else if (percent >= 20 && percent < 30)
    return X2SEG;
  else if (percent >= 30 && percent < 35)
    return X3SEG;
  else if (percent >= 35 && percent < 55)
    return YELSEG;
  else if (percent >= 55 && percent < 65)
    return REDSEG;
  else if (percent >= 65 && percent < 75)
    return MAGSEG;
  else if (percent >= 75 && percent < 85)
    return GRYSEG;
  else if (percent == 85)
    return GRNSEG;
  else
    return UNKSEG;
}

void game(void)
{
  int segments[22];
  unsigned int segc = 0;
  unsigned int curseg = 0;
  struct Segment *segstruct;
  struct rccoord cbefore;
  char input;
  char endingprint[30];
  unsigned int i = 0;
  unsigned int bluc = 0;
  unsigned int yelc = 0;
  for (i = 0; i < 22; i++)
  {
    if (i < 20)
      segments[i] = -1;
    else
      segments[i] = -2;
  }

  srand((unsigned int)time(NULL));
  while (segc < 20)
  {
    curseg = chooseseg(true);
    segstruct = &segmenttypes[curseg];
    _settextcolor(segstruct->color);
    _outtext("\r\n");
    _outtext(segstruct->label);
    _settextcolor(WHITE);
    _outtext(endingprint);
    _outtext("\r\n[C]atch, [I]gnore, [A]bort? Strike any key to ignore...");
    cbefore = _gettextposition();
    _settextposition(1, 1);
    for (i = 0; i < 22; i++)
    {
      if (segments[i] == -1)
        strcpy(endingprint, "0");
      else if (segments[i] == -2)
        strcpy(endingprint, " ");
      else if (strcmp(segmenttypes[segments[i]].inbar, "?") == 0)
      {
        sprintf(endingprint, "%c", random_hex(false));
        _settextcolor(segmenttypes[segments[i]].color);
      }
      else
      {
        strcpy(endingprint, segmenttypes[segments[i]].inbar);
        _settextcolor(segmenttypes[segments[i]].color);
      }
      _outtext(endingprint);
      _settextcolor(WHITE);
    }
    //_outtext('\0');
    _settextposition(cbefore.row, cbefore.col);
    input = getch();
    if (input == 'c' || input == 'C')
    {
      _outtext("C");
    reswitch:
      switch (curseg)
      {
      case BLUSEG:
        segments[segc++] = BLUSEG;
        break;
      case X2SEG:
        segments[segc++] = BLUSEG;
        segments[segc++] = BLUSEG;
        break;
      case X3SEG:
        segments[segc++] = BLUSEG;
        segments[segc++] = BLUSEG;
        segments[segc++] = BLUSEG;
        break;
      case YELSEG:
        segments[segc++] = YELSEG;
        break;
      case REDSEG:
        _outtext("\r\nDOS ERR 01");
        getch();
        _clearscreen(_GCLEARSCREEN);
        exit(1);
      case MAGSEG:
        if (segc > 0)
          segments[--segc] = -1;
        break;
      case GRYSEG:
        break;
      case GRNSEG:
        for (i = 0; i < 20; i++)
        {
          segments[i] = BLUSEG;
        }
        segc = 20;
        break;
      case UNKSEG:
        curseg = chooseseg(false);
        goto reswitch;
      }
    }
    else if (input == 'a' || input == 'A')
    {
      break;
    }
  }
  _settextcolor(WHITE);
  sprintf(endingprint, "\r\nGame finished, total bytes: %u (%u%%)\r\n", segc, segc * 5);
  _outtext(endingprint);
  if (segc == 21)
    _outtext("\r\n1 byte Overflow!");
  if (segc == 22)
    _outtext("\r\n2 byte Overflow!");
  bluc = 0;
  yelc = 0;
  for (i = 0; i < 22; i++)
  {
    if (segments[i] == BLUSEG)
      bluc++;
    if (segments[i] == YELSEG)
      yelc++;
  }
  _settextcolor(LIGHTBLUE);
  sprintf(endingprint, "\r\nGood bytes: %u (%u%%)", bluc, bluc * 5);
  _outtext(endingprint);
  _settextcolor(YELLOW);
  sprintf(endingprint, "\r\nCorrupted bytes: %u (%u%%)\r\n", yelc, yelc * 5);
  _outtext(endingprint);
  _settextcolor(WHITE);
  _outtext("\r\n[C]ontinue, [A]bort?");
  input = getch();
  if (input == 'c' || input == 'C')
    game();
  else
  {
    _outtext("\r\n");
    return;
  }
}

void runcmd(char *buf)
{
  char *cmd[64];
  // char outbuf[128];
  int n, i;
  n = strsplit(buf, " ", cmd, 64);
  if (n == 0 || cmd[0] == NULL)
    return;
  if (strcmp(cmd[0], "echo") == 0)
  {
    _outtext("\r\n");
    for (i = 1; i < 64; i++)
    {
      if (cmd[i] == NULL)
        break;
      _outtext(cmd[i]);
      _outtext(" ");
    }
    _outtext("\r\n");
  }
  else if (strcmp(cmd[0], "ver") == 0)
    _outtext("\r\n\r\nPB-DOS Version 3.30\r\n");
  else if (strcmp(cmd[0], "game") == 0)
    game();
  else if (strcmp(cmd[0], "exit") == 0)
  {
    _outtext("\r\n");
    exit(1);
  }
  else if (strcmp(cmd[0], "help") == 0)
  {
    _outtext("'game' - start the game.\r\n");
    _outtext("'cls' - clear screen.\r\n");
    _outtext("'ver' - show version details.\r\n");
    _outtext("'exit' - quit PB-DOS.\r\n");
  }
  else
  {
    _outtext("\r\nBad command or file name\r\n");
  }
}
void backspace(void)
{
  struct rccoord pos;
  pos = _gettextposition();
  if (pos.col > 1)
  {
    _settextposition(pos.row, pos.col - 1);
    _outtext(" ");
    _settextposition(pos.row, pos.col - 1);
  }
  else if (pos.row > 1)
  {
    /* move to end of previous row */
    _settextposition(pos.row - 1, 80);
    _outtext(" ");
    _settextposition(pos.row - 1, 80);
  }
}
void showprompt(char *prompt)
{
  char buf[128] = {0};
  unsigned int bufl = 0;
  char ch;
  bool running = true;
  char outtmp[2] = {' ', '\0'};
  bool previously_returned = true;

  while (running)
  {
    if (previously_returned)
    {
      _outtext(prompt);
      previously_returned = false;
    }
    ch = getch();
    if (ch >= 32 && bufl < 127)
    {
      outtmp[0] = ch;
      _outtext(outtmp);
      buf[bufl++] = ch;
      buf[bufl] = '\0';
    }
    else if (ch == '\b' && bufl > 0)
    {
      backspace();
      buf[--bufl] = '\0';
    }
    else if (ch == '\r')
    {
      _outtext("\r\n");
      runcmd(buf);
      previously_returned = true;
      memset(buf, 0, sizeof(buf));
      bufl = 0;
    }
  }
}

int main(int argc, char *argv[])
{
  unsigned short mem;
  char membuf[16];
  unsigned short curmem;
  bool debugging = false;
  int i;

  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "/d") == 0)
      debugging = true;
  }

  _setvideomode(_TEXTC80);
  _settextcolor(WHITE);
  _clearscreen(_GCLEARSCREEN);

  _outtext("8088-BIOS (C) 1985,1986 PMI.\r\n");
  if (debugging)
    _outtext("Debug mode.\r\n");
  _outtext("\r\n");

  mem = _bios_memsize();
  curmem = 0;

  while (curmem <= mem && !debugging)
  {
    sprintf(membuf, "%u KB", curmem);
    _outtext(membuf);
    _outtext("\r");
    curmem += 32;
    pop();
    delay(50);
  }
  _outtext("\r\n");
  _outtext("OK");
  play_soundf(1000, 30);
  delay(500);
  _clearscreen(_GCLEARSCREEN);
  delay(500);
  _outtext("Progressbar(R) PB-DOS(R) Version 3.315\r\n");
  _outtext("               (C)Copyright Progressbar Corp 1981-1987\r\n");
  _outtext("Write 'help' and strike enter to get command info.\r\n");
  showprompt("A>");
  getch();
  return 0;
}
