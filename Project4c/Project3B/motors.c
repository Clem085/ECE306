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
    //    backlightControl(0);

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
    //    backlightControl(0);

    //  Turn ON Left Motor
    P6SEL0 &= ~L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6OUT  |=  L_FORWARD;
    P6DIR  |=  L_FORWARD;
}

void RmotorForward(void){
    //  Select and Turn Off Backlight
    //    backlightControl(0);

    //  Turn ON Right Motor
    P6SEL0 &= ~R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6OUT  |=  R_FORWARD;
    P6DIR  |=  R_FORWARD;
}



// STOP Commands
void LRmotorStop(void){
    //  Select and Turn On Backlight
    //    backlightControl(1);

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
    //    backlightControl(1);

    //  Turn OFF Left Motor
    P6SEL0 &= ~L_FORWARD;
    P6SEL1 &= ~L_FORWARD;
    P6OUT  &= ~L_FORWARD;
    P6DIR  &= ~L_FORWARD;
}

void RmotorStop(void){
    //  Select and Turn On Backlight
    //    backlightControl(1);

    //  Turn OFF Right Motors
    P6SEL0 &= ~R_FORWARD;
    P6SEL1 &= ~R_FORWARD;
    P6OUT  &= ~R_FORWARD;
    P6DIR  &= ~R_FORWARD;
}





// Shape Commands
void circle(void){
    int circleCount = 0;
    while(circleCount < 200){
        int count = 0;
        //    LRmotorForward();
        //    backlightControl(1);
        while(count < 5000){
            ++count;
            RmotorForward();
            LmotorStop();
        }
        count = 0;
        while(count < 1000){
            ++count;
            LRmotorForward();
        }
        //    backlightControl(0);
        //    rightTurn();
        circleCount++;
    }
    LRmotorStop();
}



void triangle(void){
    int count = 0;
    int count2 = 0;
    //    Pivot
    int i;
    for(i = 0; i < 3; i++){
        count = 0;
        while(count < 32000){
            RmotorStop();
            LmotorForward();
            count2 = 0;
            count++;
            while((count2 < 28)){  // 27 undershoots
                count2++;
            }
        }

        count = 0;
        while(count < 32000){
            LRmotorForward();
            count++;
            count2 = 0;
            while((count2 < 100)){
                count2++;
            }
        }
        LRmotorStop();
    }
    LRmotorStop();
}

void figure8(void){
    int circleCount = 0;
    while(circleCount < 100){
        int count = 0;
        //    LRmotorForward();
        //    backlightControl(1);
        while(count < 5000){
            ++count;
            RmotorForward();
            LmotorStop();
        }
        count = 0;
        while(count < 1000){
            ++count;
            LRmotorForward();
        }
        //    backlightControl(0);
        //    rightTurn();
        circleCount++;
    }
    LRmotorStop();

    circleCount = 0;
    while(circleCount < 100){
        int count = 0;
        //    LRmotorForward();
        //    backlightControl(1);
        while(count < 5000){
            ++count;
            LmotorForward();
            RmotorStop();
        }
        count = 0;
        while(count < 1000){
            ++count;
            LRmotorForward();
        }
        //    backlightControl(0);
        //    rightTurn();
        circleCount++;
    }
    LRmotorStop();
}
