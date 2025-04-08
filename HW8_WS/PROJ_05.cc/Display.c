//------------------------------------------------------------------------------
//  Description: Adjusts display titles and state machine according to user input
//
//  Corinne McGuire
//  Date: 9/16 Fall 2024
//  Built with Code Composer Version: CCS12.4.0.00007_win64
//------------------------------------------------------------------------------

int lcd_shape = 0;

#include "macros.h"
#include "msp430.h"
#include "functions.h"
#include "LCD.h"
#include "ports.h"

//void Diplay_
void Display_Process(void);
void Carlson_StateMachine(void);

// Global variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned int display_changed;
extern volatile unsigned int update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;

unsigned int wheel_move;
char forward;

//------------------------------------------------------------------------------
// Name: Display_Process()
// Description: Updates LCD Display text
//------------------------------------------------------------------------------
void Display_Process(void){ //Function aimed to properly update display
    if(update_display){
      switch(lcd_shape){
      case 1:
          strcpy(display_line[0], "          ");
          strcpy(display_line[1], "  CIRCLE  ");
          strcpy(display_line[2], "          ");
          strcpy(display_line[3], "          ");
      break;
      case 2:
          strcpy(display_line[0], "          ");
          strcpy(display_line[1], "  FIGURE  ");
          strcpy(display_line[2], "   EIGHT  ");
          strcpy(display_line[3], "          ");
      break;
      case 3:
          strcpy(display_line[0], "          ");
          strcpy(display_line[1], " TRIANGLE ");
          strcpy(display_line[2], "          ");
          strcpy(display_line[3], "          ");
      break;
      default: break;
      }
    update_display = 0;
  }
    if(display_changed){
        display_changed = 0;
        Display_Update(0,0,0,0);
    }
}

void Display_Change(lcdstate){
    update_display = 1;
    lcd_shape = lcdstate;
    display_changed = TRUE;
    Display_Process();
}

//------------------------------------------------------------------------------
// Name: Carlson-StateMachine()
// Description: Updates car state
//------------------------------------------------------------------------------
void Carlson_StateMachine(void){
    switch(Time_Sequence){
      case 250:                        //
        if(one_time){
          Init_LEDs();
          lcd_BIG_mid();
          display_changed = 1;
          one_time = 0;
        }
        Time_Sequence = 0;             //
        break;
      case 200:                        //
        if(one_time){
          P6OUT |= GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 150:                         //
        if(one_time){
          P1OUT |= RED_LED;            // Change State of LED 4
          P6OUT &= ~GRN_LED;            // Change State of LED 5
          one_time = 0;
        }
        break;
      case 100:                         //
        if(one_time){
          lcd_BIG_bot();
          P6OUT |= GRN_LED;            // Change State of LED 5
          display_changed = 1;
          one_time = 0;
        }
        break;
      case  50:                        //
        if(one_time){
          one_time = 0;
        }
        break;                         //
      default: break;
    }
}
