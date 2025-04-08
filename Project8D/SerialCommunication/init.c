/* Initialize Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : init.c
  Description:  This file contains the Initialization sequences for many of the boards and processes used in this project.
  Programmer: Connor Savugot
  Date Created: Sep 12, 2024
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
*/

// #include as of 11-10-24
//// Header Files
#include  "msp430.h"
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include  "macros.h"
#include  "motors.h"
#include  "Display.h"
#include  "timers.h"
#include  "interrupts.h"
#include  "switches.h"
#include  "ADC.h"
#include  "IR.h"
#include  "serial.h"
#include  "DAC.h"
#include  "menu.h"
//// Libraries
#include  <string.h>
#include  <stdio.h>


//Variables
// No Header File.
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char update_display;
extern volatile unsigned char display_changed;
unsigned char display_mode;
extern unsigned int count_debounce_SW1;
extern unsigned int count_debounce_SW2;
extern char IR_status;
extern char ADC_Update;
extern char ADC_Display;
extern char state;
extern char light_percent;
extern char DAC_overflow_counter;


void Init_Conditions(void){
//------------------------------------------------------------------------------

  int i;
  for(i=0;i<11;i++){
    display_line[0][i] = RESET_STATE;
    display_line[1][i] = RESET_STATE;
    display_line[2][i] = RESET_STATE;
    display_line[3][i] = RESET_STATE;
  }
  display_line[0][10] = 0;
  display_line[1][10] = 0;
  display_line[2][10] = 0;
  display_line[3][10] = 0;

  display[0] = &display_line[0][0];
  display[1] = &display_line[1][0];
  display[2] = &display_line[2][0];
  display[3] = &display_line[3][0];
  update_display = 0;


// Interrupts are disabled by default, enable them.
  enable_interrupts();

  count_debounce_SW1 = 0;
  count_debounce_SW2 = 0;
  DAC_overflow_counter = 0;

  P1OUT  &= ~RED_LED; // Set Red LED Off
  P6OUT  &= ~GRN_LED; // Set Green LED Off

//------------------------------------------------------------------------------
}
