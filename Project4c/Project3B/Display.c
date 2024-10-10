#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}

// Controls Display Backlight
void backlightControl(int action){
/*  Parameter Values
    action
        0: Backlight OFF
        1: Backlight ON
*/
    switch(action){
    case 0: // Turn Backlight OFF
        P6SEL0 &= ~LCD_BACKLITE;
        P6SEL1 &= ~LCD_BACKLITE;
        P6OUT  &= ~LCD_BACKLITE;
        P6DIR  &= ~LCD_BACKLITE;
        break;
    case 1: // Turn Backlight ON
        P6SEL0 &= ~LCD_BACKLITE;
        P6SEL1 &= ~LCD_BACKLITE;
        P6OUT  |=  LCD_BACKLITE;
        P6DIR  |=  LCD_BACKLITE;
        break;
    default:
        // Display ERROR on Screen

        // Turn Display ON
        P6SEL0 &= ~LCD_BACKLITE;
        P6SEL1 &= ~LCD_BACKLITE;
        P6OUT  |=  LCD_BACKLITE;
        P6DIR  |=  LCD_BACKLITE;
        break;
    }
}
