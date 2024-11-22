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
char RnW[28] = {"Red and White Red and White "};
char WnR[28] = {"White and Red White and Red "};
char Song[466] = {"We're the Red and White from State "
                  "And we know we are the best. "
                  "A hand behind our back, "
                  "We can take on all the rest. "
                  "Come over the hill, Carolina. "
                  "Devils and Deacs stand in line. "
                  "The Red and White from N.C. State. "
                  "Go State!"
                  "We're the Red and White from State "
                  "And we know we are the best. "
                  "A hand behind our back, "
                  "We can take on all the rest. "
                  "Come over the hill, Carolina. "
                  "Devils and Deacs stand in line. "
                  "The Red and White from N.C. State. "
                  "Go State!"};

// Functions
void menuScroll(void){
    switch(menuType){
    case IDLE:
        lcd_BIG_mid();
        strcpy(display_line[0], "  Connor  ");
        strcpy(display_line[1], "Homework 9");
        strcpy(display_line[2], " Savugot  ");
        display_changed = TRUE;
        break;
    case OUTER_MENU:{
        lcd_BIG_mid();
        ADC_Outer = ADC_Thumb >> 8; // Leaves 2 Bits Remaining
        switch(ADC_Outer){
        case 0x00: // Resistor
            menu = RESISTOR;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], " RESISTOR ");
            strcpy(display_line[2], "          ");
            display_changed = TRUE;
            break;
        case 0x01: // Shapes
            menu = SHAPES;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "  SHAPES  ");
            strcpy(display_line[2], "          ");
            display_changed = TRUE;
            break;
        case 0x02: // Song
            menu = SONG;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "   SONG   ");
            strcpy(display_line[2], "          ");
            display_changed = TRUE;
            break;
        default:
            // Just to Cover Undefined Bit Values. With 2 Bits, Cases 0:4 are available, but only Cases 0:3 are used
            menu = SONG;
            strcpy(display_line[0], "          ");
            strcpy(display_line[1], "   SONG   ");
            strcpy(display_line[2], "          ");
            display_changed = TRUE;
            break;
        }
        break;}
    case INNER_MENU:
        switch(menu){
        // ADC_RESOLUTION - floor(log_2(#ItemsInList)) = #Right_Shift
        case RESISTOR:
            lcd_4line();
            ResistorMenu();
            break;
        case SHAPES:
            lcd_BIG_mid();
            ShapeMenu();
            break;
        case SONG:
            lcd_BIG_mid();
            SongMenu();
            break;
        default:
            break;
        }
        break;
        default:
            break;
    }
}

void ResistorMenu(void){
    //    strcpy(display_line[0], "   Color  ");
    //    strcpy(display_line[2], "   Value  ");
    dispPrint("Color",'1');
    dispPrint("Value",'3');
    display_changed = TRUE;
    // 10 - Ceiling(log_2(10)) = 7
    // 10 - 4 = 6
    ADC_Inner = ADC_Thumb >> 6; // Leaves 4 Bits Remaining
    switch(ADC_Inner){
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
        // Just to Cover Undefined Bit Values. With 4 Bits, Cases 0:15 are available, but only Cases 0:9 are used
        dispPrint("White", '2');
        value[0] = WHITE + '0';
        value[1] = '\0';
        dispPrint(value, '4');
        break;
    }

}

void ShapeMenu(void){
    // ADC_RESOLUTION - floor(log_2(#ItemsInList)) = #Right_Shift
    // 10 - Ceiling(log_2(10)) = 7
    // 10 - 4 = 6
    ADC_Inner = ADC_Thumb >> 6; // Leaves 4 Bits Remaining
    switch(ADC_Inner){
    case CIRCLE:
        strcpy(display_line[0], " CYLINDER ");
        strcpy(display_line[1], "  CIRCLE  ");
        strcpy(display_line[2], "  SQUARE  ");
        display_changed = TRUE;
        break;

    case SQUARE:
        strcpy(display_line[0], "  CIRCLE  ");
        strcpy(display_line[1], "  SQUARE  ");
        strcpy(display_line[2], " TRIANGLE ");
        display_changed = TRUE;
        break;

    case TRIANGLE:
        strcpy(display_line[0], "  SQUARE  ");
        strcpy(display_line[1], " TRIANGLE ");
        strcpy(display_line[2], " OCTAGON  ");
        display_changed = TRUE;
        break;

    case OCTAGON:
        strcpy(display_line[0], " TRIANGLE ");
        strcpy(display_line[1], " OCTAGON  ");
        strcpy(display_line[2], " PENTAGON ");
        display_changed = TRUE;
        break;

    case PENTAGON:
        strcpy(display_line[0], " OCTAGON  ");
        strcpy(display_line[1], " PENTAGON ");
        strcpy(display_line[2], " HEXAGON  ");
        display_changed = TRUE;
        break;

    case HEXAGON:
        strcpy(display_line[0], " PENTAGON ");
        strcpy(display_line[1], " HEXAGON  ");
        strcpy(display_line[2], "   CUBE   ");
        display_changed = TRUE;
        break;

    case CUBE:
        strcpy(display_line[0], " HEXAGON  ");
        strcpy(display_line[1], "   CUBE   ");
        strcpy(display_line[2], "   OVAL   ");
        display_changed = TRUE;
        break;

    case OVAL:
        strcpy(display_line[0], "   CUBE   ");
        strcpy(display_line[1], "   OVAL   ");
        strcpy(display_line[2], "  SPHERE  ");
        display_changed = TRUE;
        break;

    case SPHERE:
        strcpy(display_line[0], "   OVAL   ");
        strcpy(display_line[1], "  SPHERE  ");
        strcpy(display_line[2], " CYLINDER ");
        display_changed = TRUE;
        break;

    case CYLINDER:
        strcpy(display_line[0], "  SPHERE  ");
        strcpy(display_line[1], " CYLINDER ");
        strcpy(display_line[2], "  CIRCLE  ");
        display_changed = TRUE;
        break;
    default:
        // Just to Cover Undefined Bit Values. With 4 Bits, Cases 0:15 are available, but only Cases 0:9 are used
        strcpy(display_line[0], "  SPHERE  ");
        strcpy(display_line[1], " CYLINDER ");
        strcpy(display_line[2], "  CIRCLE  ");
        display_changed = TRUE;
        break;
    }
}


void SongMenu(void){
//    char RnW2[28];
//    char Song2[446];
//    char WnR2[28];
    if(songInit){
        songInit = FALSE;
        strncpy(display_line[0], RnW, 10);
        strncpy(display_line[1], Song, 10);
        strncpy(display_line[2], WnR, 10);
        i = 0;
        j = 0;
//        strncpy(RnW2,RnW,10);
//        strncat(RnW2,RnW,10);
//        strncpy(Song2,Song);
//        strncat(Song2,Song);
//        strncpy(WnR2,WnR);
//        strncat(WnR2,WnR);
    }
    if(ADC_Temp > ADC_Prev){
        i++;
        j++;
        if(i < 14-5){
            strncpy(display_line[0], RnW+i, 10);
            strncpy(display_line[2], WnR+i, 10);
        }else{
            i = 0;
        }
        if(j < 223-5){
            strncpy(display_line[1], Song+j, 10);
        }else{
            j = 0;
        }
    }
    display_changed = TRUE;
    ADC_Prev = ADC_Temp;
}


//    if(ADC_Temp > ADC_Prev || songInit == TRUE){
//        P1OUT ^= RED_LED;


//        if(i<5){
//            strncpy(display_line[0], RnW+i, 10);
//            strncpy(display_line[2], WnR+i, 10);
//            i++;
//        }else if(i>=5){
//            songInit = FALSE;
//            strncpy(display_line[0], RnW+i, 14-i);
//            strncpy(display_line[0], RnW, i-4);
//            strncpy(display_line[2], WnR+i, 14-i);
//            strncpy(display_line[2], WnR, i-4);
//            i++;
//            if(i>=14){
//                i = 0;
//            }
//        }
//        strncpy(display_line[1], Song+j,10);
//        if(j<(223-10)){
//            j++;
//        }
//    }




