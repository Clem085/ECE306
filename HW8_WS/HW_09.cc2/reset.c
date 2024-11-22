#include  "ports.h"
#include  "macros.h"
#include "msp430.h"
#include "functions.h"

void Reset(void){
    if (((LEFT_FORWARD_SPEED) != 0 ) && ((LEFT_REVERSE_SPEED) != 0 )) {

         RIGHT_FORWARD_SPEED = WHEEL_OFF;
         LEFT_FORWARD_SPEED = WHEEL_OFF;
         RIGHT_REVERSE_SPEED = WHEEL_OFF;
         LEFT_REVERSE_SPEED = WHEEL_OFF;

         P1OUT |= RED_LED; // Set Value = On / High

    }
    if (((RIGHT_FORWARD_SPEED) != 0 ) && ((RIGHT_REVERSE_SPEED) != 0 )){

         RIGHT_FORWARD_SPEED = WHEEL_OFF;
         LEFT_FORWARD_SPEED = WHEEL_OFF;
         RIGHT_REVERSE_SPEED = WHEEL_OFF;
         LEFT_REVERSE_SPEED = WHEEL_OFF;

        P1OUT |= RED_LED; // Set Value = On / High

        }
}
