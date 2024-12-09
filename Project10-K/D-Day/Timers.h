//===========================================================================
//  File Name: timers.h
//  Description: Header file for timers.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Oct 9, 2024
//===========================================================================
#ifndef TIMERS_H_
#define TIMERS_H_


// Global Variables
//// Internally Defined

//// Externally Defined




// Macro Definitions
// TIMERS
#define TB0CCR0_INTERVAL (6250)
#define TB0CCR1_INTERVAL (50000)
#define TB0CCR2_INTERVAL (625) // Temp Change to 125 to make ADC update every 10 ms, OLD=2500


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


#endif /* TIMERS_H_ */
