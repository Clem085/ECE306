//===========================================================================
//  File Name: timers.h
//  Description: Header file for timers.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Oct 9, 2024
//===========================================================================
#ifndef TIMERSB0_H_
#define TIMERSB0_H_

// Global Variables
//// Internally Defined
volatile char one_time;
unsigned int counter_B0;
unsigned int delay_time;
//// Externally Defined
extern volatile unsigned int Time_Sequence;
extern char backlight_status;
extern int Switch_Counter1;
extern volatile unsigned char update_display;
extern int activateSM;
extern char display_line[4][11];
extern volatile unsigned char display_changed;




// Macro Definitions
// TIMERS
//    // Timer A0
//#define TB0CCR0_INTERVAL (25000) // 8,000,000 / 8 / 8 / (1000 / 200msec)
//    // Others
//#define TB0CCR1_INTERVAL (25000) // 8,000,000 / 8 / 8 / (1000 / 200msec)
//#define TB0CCR2_INTERVAL (25000) // 8,000,000 / 8 / 8 / (1000 / 200msec)

/// /// /// TEMP PROJECT 8 CODE /// /// ///
#define TB0CCR0_INTERVAL (500)
#define TB0CCR1_INTERVAL (500)
#define TB0CCR2_INTERVAL (500)
/// /// /// TEMP PROJECT 8 CODE /// /// ///

    // Timer B3
#define TB3CCR0_INTERVAL (5000)
#define TIMER_B0_0_VECTOR (TIMER0_B0_VECTOR)
#define TIMER_B0_1_OVFL_VECTOR (TIMER0_B1_VECTOR)
#define TIMER_B3_0_VECTOR (TIMER0_B3_VECTOR)



// Function Declarations
void Init_ADC(void);
void HexToBCD(int);
// Timers
void Init_Timers(void);
void Init_Timer_B0(void);
void Init_Timer_B1(void);
void Init_Timer_B2(void);
void Init_Timer_B3(void);
void half_sec_delay(void);



#endif /* TIMERSB0_H_ */
