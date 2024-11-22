//------------------------------------------------------------------------------
//
//  Description: global state variables used for Project
//
//  Corinne McGuire
//  Date: 9/16 Fall 2024
//  Built with Code Composer Version: CCS12.4.0.00007_win64

#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //

// STATES ======================================================================
#define NONE ('N')
#define STRAIGHT ('L')
#define CIRCLE ('C')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')
#define PRESSED (1)
#define RELEASED (0)
#define NOT_OKAY (0)
#define OKAY (1)
#define DEBOUNCE_RESTART (0)
#define WHEEL_COUNT_TIME (600)
#define RIGHT_COUNT_TIME (600)
#define LEFT_COUNT_TIME (600)
#define TRAVEL_DISTANCE (200)
#define WAITING2START (75)
#define P4PUD (P4OUT)
#define SLOW (15000)

// EXTERNAL VARIABLES
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned int display_changed;
extern volatile unsigned int update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
extern volatile unsigned int Last_Time_Sequence; // a variable to identify Time_Sequence has changed
extern unsigned int cycle_time; // is a new time base used to control making shapes
extern unsigned int time_change; // is an identifier that a change has occurred
extern volatile char one_time;
extern volatile char event;
extern volatile char state;
extern volatile int shape;
extern volatile int finished;
extern volatile int sw1_position;
extern volatile int sw2_position;
extern volatile int sw1_counter;
extern volatile int sw2_counter;

volatile int ADC_Left_Detect;
volatile int ADC_Right_Detect;
volatile int ADC_Thumb;

extern unsigned int DAC_data;

#define STILL (0)
#define WAIT_NOW (1)
#define MOVE_FORWARD (2)
#define TURN (3)
#define STOP (4)

// Totla milliseconds in a second: 1,000,000
// Divided by 8 => 1,000,000 / 8 = 125,000
// Then divided by 3 => 125,000 / 3 = 41,666.6667
#define TB0CCR0_INTERVAL (500)
#define TB0CCR1_INTERVAL (500)
#define TB0CCR2_INTERVAL (500)

#define PWM_PERIOD TB3CCR0  // Assign PWM_PERIOD to the correct timer register
#define LCD_BACKLITE_DIMING TB3CCR1  // Set this to control duty cycle

#define PERCENT_80 (WHEEL_PERIOD * 0.8)  // 80% duty cycle

#define P4PUD (P4OUT)
#define P2PUD (P2OUT)
// Example values for other constants

#define BEGINNING (0)
#define SMALL_RING_SIZE (16)
#define LARGE_RING_SIZE (32)
