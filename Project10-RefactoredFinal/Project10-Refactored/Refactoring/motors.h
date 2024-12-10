//===========================================================================
//  File Name: motors.h
//  Description: Header file for motors.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Nov 10, 2024
//===========================================================================

#ifndef MOTORS_H_
#define MOTORS_H_

// Global Variables
//// Internally Defined
////// Counters
unsigned int Time_Sequence;
////// Arch Variables
unsigned int arch_counter;
unsigned int archNext;
unsigned int archState;
unsigned int nextState;
unsigned int motorDrain;
unsigned int init_cmd_state;
////// Black Line
unsigned int following;

//// Externally Defined
extern char command;
extern unsigned int moving;
extern unsigned int moving_flag;
extern unsigned int movement_flag;
extern unsigned int stopMoving;
extern char display_line[4][11];
extern char *display[4];
extern unsigned char display_changed;
extern unsigned int ADC_Left_Detect;
extern unsigned int ADC_Right_Detect;
extern unsigned int pause_flag;
extern unsigned int state;
extern unsigned int black_low;
extern unsigned int black_high;




// Macro Definitions
//// Old #defines
#define NONE ('N')
#define RIGHT (1)
#define LEFT (0)
#define PAUSE (200) // originally 20, changed to 200 for TA Demo
#define RIGHTARCHFWD (26000) //26000
#define LEFTARCHFWD (20000)  //20000
#define RSLOWCIRCLE (35000) // 35000
#define LSLOWCIRCLE (25000) // 25000

// PWM1 Values
// REMEMBER
    // Reverse or Forward must be 0 when the other is not 0.
    // DESIRED ON AMOUNT must be a value less than WHEEL_PERIOD
    // If WHEEL_PERIOD is 50,005 then
    //DESIRED ON AMOUNT of 20,000 would be ON NEAR 0%
//// PWM Off
#define WHEEL_OFF      (00000)
//// Slow PWM Values
#define SLOW_FWD_RIGHT (20000)
#define SLOW_FWD_LEFT  (20000)
#define SLOW_REV_RIGHT (20000)
#define SLOW_REV_LEFT  (20000)
#define SLOW_SPIN      (15000)
//// Fast PWM Values
#define FAST_FWD_RIGHT (35000)
#define FAST_FWD_LEFT  (35000)
#define FAST_REV_RIGHT (35000)
#define FAST_REV_LEFT  (35000)


// Function Declarations
//// PWM
void motors_off(void);
void fwd_off(void);
void rev_off(void);
void spin_left_off(void);
void spin_right_off(void);
void fwd_slow(void);
void rev_slow(void);
void left_slow(void);
void right_slow(void);
void fwd_fast(void);
void rev_fast(void);
void left_fast(void);
void right_fast(void);
void spin_left(void);
void spin_right(void);
//// Project 10 Movements
void arch_movement(void);
void black_line(void);
void initialMovementBL(void);
//// Black Line FOLLOWing
void line_search(void);
void line_found(void);
void parallel_park(void);
void follow_pause(void);
void follow_line(void);

//// Magic Smoke Detection
void vrfyDirection(void);






#endif /* MOTORS_H_ */
