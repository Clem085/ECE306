//------------------------------------------------------------------------------
//
//  Description: This file turns on the display screen
//
//  Salena Price
//  Jan 2025

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#include  "msp430.h"
#include  <string.h>
#include  "include\functions.h"
#include  "Include\LCD.h"
#include  "include\ports.h"
#include "macros.h"

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern char display_line[4][11];
extern char *display[4];
extern unsigned int event;


void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}

void Display_SWITCH1(void){
      strcpy(display_line[0], "SWITCH 1  ");
      strcpy(display_line[1], "   PRESSED   ");
      strcpy(display_line[2], "             ");
      strcpy(display_line[3], "             ");
      display_changed = TRUE;
      Display_Update(0,0,0,0);
    }

void Display_SWITCH2(void){
      strcpy(display_line[0], "SWITCH 2  ");
      strcpy(display_line[1], "   PRESSED   ");
      strcpy(display_line[2], "             ");
      strcpy(display_line[3], "             ");
      display_changed = TRUE;
      Display_Update(0,0,0,0);
    }

void Display_EMITON(void){
      strcpy(display_line[0], "             ");
      strcpy(display_line[1], "             ");
      strcpy(display_line[2], "             ");
      strcpy(display_line[3], "   EMIT ON   ");
      display_changed = TRUE;
      Display_Update(0,0,0,0);
    }

void Display_EMITOFF(void){
      strcpy(display_line[0], "             ");
      strcpy(display_line[1], "             ");
      strcpy(display_line[2], "             ");
      strcpy(display_line[3], "  EMIT OFF   ");
      display_changed = TRUE;
      Display_Update(0,0,0,0);
    }

void Display_FOUND(void){
      strcpy(display_line[0], "             ");
      strcpy(display_line[1], "             ");
      strcpy(display_line[2], "  FOUND    ");
      strcpy(display_line[3], "             ");
      display_changed = TRUE;
      Display_Update(0,0,0,0);
    }

void Display_WAIT(void){
      strcpy(display_line[0], "             ");
      strcpy(display_line[1], "             ");
      strcpy(display_line[2], "    WAIT    ");
      strcpy(display_line[3], "             ");
      display_changed = TRUE;
      Display_Update(0,0,0,0);
    }

void Display_SEARCHING(void){
      strcpy(display_line[0], "             ");
      strcpy(display_line[1], "             ");
      strcpy(display_line[2], "SEARCHING  ");
      strcpy(display_line[3], "             ");
      display_changed = TRUE;
      Display_Update(0,0,0,0);
    }

void Display_TURNING(void){
      strcpy(display_line[0], "             ");
      strcpy(display_line[1], "             ");
      strcpy(display_line[2], "TURNING   ");
      strcpy(display_line[3], "             ");
      display_changed = TRUE;
      Display_Update(0,0,0,0);
    }

void Display_DONE(void){
      strcpy(display_line[0], "             ");
      strcpy(display_line[1], "             ");
      strcpy(display_line[2], "     DONE    ");
      strcpy(display_line[3], "             ");
      display_changed = TRUE;
      Display_Update(0,0,0,0);
    }
