#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

//void LRmotorForward(void){
//    //    Select and Turn Off Backlight
//    blacklightControl(0);
//
//    //  Turn ON Motors
//    P6SEL0 &= ~R_FORWARD;
//    P6SEL1 &= ~R_FORWARD;
//    P6OUT  |=  R_FORWARD;
//    P6DIR  |=  R_FORWARD;
//
//    P6SEL0 &= ~L_FORWARD;
//    P6SEL1 &= ~L_FORWARD;
//    P6OUT  |=  L_FORWARD;
//    P6DIR  |=  L_FORWARD;
//}
//
//void LRmotorStop(void){
//    //    Select and Turn On Backlight
//    backlightControl(1);
//
//    //  Turn OFF Motors
//    P6SEL0 &= ~R_FORWARD;
//    P6SEL1 &= ~R_FORWARD;
//    P6OUT  &= ~R_FORWARD;
//    P6DIR  &= ~R_FORWARD;
//
//    P6SEL0 &= ~L_FORWARD;
//    P6SEL1 &= ~L_FORWARD;
//    P6OUT  &= ~L_FORWARD;
//    P6DIR  &= ~L_FORWARD;
//}
