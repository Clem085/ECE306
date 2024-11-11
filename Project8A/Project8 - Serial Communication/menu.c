/* Menu Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : menu.c
  Description:  This file contains the code to control Menu on LCD Display (Homework 9)
  Programmer: Connor Savugot
  Date Created: Nov 10, 2024
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

// Global Variables declared and referenced in Header file


// Functions
void menuSelect(void){
    switch(menu){
    case IDLE:
        strcpy(display_line[0], "          ");
        strcpy(display_line[1], "  Press a ");
        strcpy(display_line[2], " Switch to");
        strcpy(display_line[3], "   Begin  ");
        display_changed = TRUE;
        break;
    case RESISTOR:
//        strcpy(display_line[0], " RESISTOR ");
//        display_changed = TRUE;
        break;
    case SHAPES:
//        strcpy(display_line[0], "  SHAPES  ");
//        display_changed = TRUE;
        break;
    case SONG:
//        strcpy(display_line[0], "   SONG   ");
//        display_changed = TRUE;
        break;
    default:
        break;
    }
}

void menuScroll(void){
    switch(menu){
    // ADC_RESOLUTION - floor(log_2(#ItemsInList)) = #Right_Shift
    case IDLE:
        break;
    case RESISTOR:
        strcpy(display_line[0], "   Color  ");
        strcpy(display_line[2], "   Value  ");
        display_changed = TRUE;
        // 10 - Ceiling(log_2(10)) = 7
        // 10 - 4 = 6
        ADC_Thumb = ADC_Thumb >> 6; // Leaves 4 Bits Remaining
        // Temp Test Code
        HexToBCD(ADC_Thumb);
        dispPrint(adc_char,'3');

        switch(ADC_Thumb){
        char value[2];
        case BLACK:
            dispPrint("Black",'2');
            value[0] = BLACK+'0';
            value[1] = '\0';
            dispPrint(value,'4');
            break;
        case BROWN:
            dispPrint("Brown",'2');
            value[0] = BROWN+'0';
            value[1] = '\0';
            dispPrint(value,'4');
            break;
        case RED:
            dispPrint("Red",'2');
            value[0] = RED+'0';
            value[1] = '\0';
            dispPrint(value,'4');
            break;
        case ORANGE:
            dispPrint("Orange",'2');
            value[0] = ORANGE+'0';
            value[1] = '\0';
            dispPrint(value,'4');
            break;
        case YELLOW:
            dispPrint("Yellow",'2');
            value[0] = YELLOW+'0';
            value[1] = '\0';
            dispPrint(value,'4');
            break;
        case GREEN:
            dispPrint("Green",'2');
            value[0] = GREEN+'0';
            value[1] = '\0';
            dispPrint(value,'4');
            break;
        case BLUE:
            dispPrint("Blue", '2');
            value[0] = BLUE + '0';
            value[1] = '\0';
            dispPrint(value, '4');
            break;
        case VIOLET:
            dispPrint("Violet", '2');
            value[0] = VIOLET + '0';
            value[1] = '\0';
            dispPrint(value, '4');
            break;
        case GRAY:
            dispPrint("Gray", '2');
            value[0] = GRAY + '0';
            value[1] = '\0';
            dispPrint(value, '4');
            break;
        case WHITE:
            dispPrint("White", '2');
            value[0] = WHITE + '0';
            value[1] = '\0';
            dispPrint(value, '4');
            break;
        default:
            break;
        }
        break;
    case SHAPES:
        // ADC_Thumb = ADC_Thumb;
        // 10Bit_ADC - 10List_Items = 0Bit_Shift
        strcpy(display_line[0], "  SHAPES  ");
        display_changed = TRUE;
        break;
    case SONG:
        strcpy(display_line[0], "   SONG   ");
        display_changed = TRUE;
        break;
    default:
        break;
    }
}

