#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

/* Functions Included in this File
    LRmotorForward
    LmotorForward
    RmotorForward

    LRmotorStop
    LmotorStop
    RmotorStop
*/

// FORWARD Commands
void LRmotorForward(void){
    //  Select and Turn Off Backlight
    backlightControl(0);

    //  Turn ON Motors
    P6SEL0 &= ~R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6OUT  |=  R_FORWARD;
    P6DIR  |=  R_FORWARD;

    P6SEL0 &= ~L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6OUT  |=  L_FORWARD;
    P6DIR  |=  L_FORWARD;
}

void LmotorForward(void){
    //  Select and Turn Off Backlight
    backlightControl(0);

    //  Turn ON Left Motor
    P6SEL0 &= ~L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6OUT  |=  L_FORWARD;
    P6DIR  |=  L_FORWARD;
}

void RmotorForward(void){
    //  Select and Turn Off Backlight
    backlightControl(0);

    //  Turn ON Right Motor
    P6SEL0 &= ~R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6OUT  |=  R_FORWARD;
    P6DIR  |=  R_FORWARD;
}



// STOP Commands
void LRmotorStop(void){
    //  Select and Turn On Backlight
    backlightControl(1);

    //  Turn OFF Motors
    P6SEL0 &= ~R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6OUT  &= ~R_FORWARD;
    P6DIR  &= ~R_FORWARD;

    P6SEL0 &= ~L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6OUT  &= ~L_FORWARD;
    P6DIR  &= ~L_FORWARD;
}

void LmotorStop(void){
    //  Select and Turn On Backlight
    backlightControl(1);

    //  Turn OFF Left Motor
    P6SEL0 &= ~L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6OUT  &= ~L_FORWARD;
    P6DIR  &= ~L_FORWARD;
}

void RmotorStop(void){
    //  Select and Turn On Backlight
    backlightControl(1);

    //  Turn OFF Right Motors
    P6SEL0 &= ~R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6OUT  &= ~R_FORWARD;
    P6DIR  &= ~R_FORWARD;
}





//// Shape Commands
//void circle(){
//    int count = 0;
//    while(count < 100){
//        rightTurn();
//        ++count;
//    }
//}
//
//void rightTurn(){
//    LRmotorForward();
//    switch(Time_Sequence){
//    case 250:
//        Time_Sequence = 0;
//        RmotorStop();
//        break;
//    case 200:
//        RmotorForward();
//        break;
//    case 150:
//        RmotorStop();
//        break;
//    case 100:
//        RmotorForward();
//        break;
//    case  50:
//        RmotorStop();
//        break;
//    default: break;
//    }
//}
//
