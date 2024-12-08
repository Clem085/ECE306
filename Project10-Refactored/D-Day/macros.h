//===========================================================================
//  File Name : macros.h
//  Description:  This file contains the macros used by all the other files
//  Author: Connor Savugot
//  Date: Sep 12, 2024
//===========================================================================

#ifndef MACROS_H_
#define MACROS_H_

#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define FALSE                (0x00)

// STATES ======================================================================

// All Deprecated
//#define NONE                  ('N')
//#define WAIT                  ('W')
//#define START                 ('S')
//#define RUN                   ('R')
//#define END                   ('E')

// EVENTS
    // Project 4
// #define NONE                  ('N')
// #define STRAIGHT              ('L')
// #define CIRCLE                ('C')
// #define TRIANGLE              ('T')
// #define TRIANGLE_CURVE        ('t')
// #define FIGURE8C1             ('F')
// #define FIGURE8C2             ('f')
    // Project 5
//#define FWD                    ('f')
//#define REV                    ('r')
//#define SPIN                   ('s')


//// Project 7 States
//#define IDLE                    ('I')
//#define WAIT                    ('W')
//#define FWD                     ('F')
//#define BLACKLINE               ('B')
//#define WAIT2                   ('w')
//#define TURNL                   ('T')
//#define LINE1                   ('1')
//#define LINE2                   ('2')
//#define DONE                    ('D')
//#define LEFT_SIDE               ('L')
//#define MIDDLE_L                ('l')
//#define RIGHT_SIDE              ('R')
//#define MIDDLE_R                ('r')

// Project 8 States
#define WAIT                    ('W')
#define RECEIVE                 ('R')
#define TRANSMIT                ('T')

// Homework 9 States


// Switches
#define PRESSED                (0)
#define RELEASED               (1)
#define NOT_OKAY               (0)
#define OKAY                   (1)
//#define DEBOUNCE_TIME          (12)
//#define DEBOUNCE_RESTART       (0)
#define WAITING2START          (500) //1000 Long
#define P4PUD (P4OUT)

#define OFF                    ('0')
#define ON                     ('1')
// #define SW1 (0x02) // Switch 1

// Port 3 Pin 4 Behavior
#define USE_GPIO    (0x00)
#define USE_SMCLK   (0x01)




#define HALF_SEC (500)
#define COUNTER_RESET (0)





#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define MCLK_FREQ_MHZ           (8) // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)

// STATES ======================================================================
// STATEMACHINE

#define DETECTED ('D')
#define SPIN ('O')
#define TRACK ('T')
#define TRACKWAIT ('K')


//#define NONE ('N')
//#define LEFT ('L')
#define CIRCLE ('C')
//#define FIGURE ('B')
#define INTERCEPT ('X')
#define IRCONF ('i')
#define SLOWRIGHT ('s')
#define TRIANGLE ('T')
#define WAIT ('W')
#define START ('S')
#define ARCH ('a')
#define RUN ('R')
#define END ('E')
#define EXIT ('e')
#define FORWARDS ('F')
//#define RIGHT ('R')
#define CLKW ('K')
#define COUNTERCLKW ('U')
#define IDLE ('I')
#define SEARCH ('Z')
#define ALIGN ('Y')
#define BACK ('B')
#define ARRIVED ('A')
#define WHEEL_COUNT_TIME (10)
#define RIGHT_COUNT_TIME (7)
#define LEFT_COUNT_TIME (8)
#define TRAVEL_DISTANCE (18)
#define WAITING2START (50)
#define CIRCLE_TRAVEL_DISTANCE (200)
#define COUNTERCLKW_RIGHT_COUNT_TIME (240)
#define COUNTERCLKW_LEFT_COUNT_TIME (1)
#define CLKW_RIGHT_COUNT_TIME (1)
#define CLKW_LEFT_COUNT_TIME (270)
#define TRIANGLE_TURN_DISTANCE (17)
#define TRITURN_RIGHT_COUNT_TIME (350)
#define TRITURN_LEFT_COUNT_TIME (0)
#define PRESSED (0)
#define RELEASED (1)
#define NOT_OKAY (0)
#define OKAY (1)
#define DEBOUNCE_RESTART (0)
#define DEBOUNCE_TIME (100)

#define TB0CCR0_INTERVAL (6250)
#define TB0CCR1_INTERVAL (50000)
#define TB0CCR2_INTERVAL (625) // Temp Change to 125 to make ADC update every 10 ms, OLD=2500


// SERIAL
#define BEGINNING (0)
#define SMALL_RING_SIZE (16)
#define LARGE_RING_SIZE (32)
//#define RECEIVE ('P')
//#define TRANSMIT ('H')
#define SSID_SIZE (10)




#define WHEEL_PERIOD (50005)
#define SMALL (15000)
#define NINETY (37515)
#define FOURFIVE (28135)
#define SLOW (20000)
#define FASTRIGHT (50000)
#define FASTLEFT (30000)
#define FAST (35000)

// NEW DISTANCE SUFF
#define TRUE_FWD_R (35000)
#define TRUE_FWD_L (35000)

#define TRUE_REV_R (35000)
#define TRUE_REV_L (35000)

#define SLOW_FWD_R (20000)
#define SLOW_FWD_L (20000)

#define SLOW_REV_R (20000)
#define SLOW_REV_L (20000)

// Black Line Follow State Durations
#define FWD1_DURATION (5)
#define FWD2_DURATION (9)
#define BACK1_DURATION (20) // was 9
#define BACK2_DURATION (30) // Was 9
#define S180_DURATION (10)
#define LEFT1_DURATION_LEFT (3)
#define LEFT1_DURATION_FWD (3)
#define LEFT2_DURATION_LEFT (5)
#define LEFT2_DURATION_FWD (5)
#define RIGHT1_DURATION_RIGHT (3) // was 5
#define RIGHT1_DURATION_FWD (3) // Was 5, too Much
#define RIGHT2_DURATION_RIGHT (5)
#define RIGHT2_DURATION_FWD (3)

// Backlight Values
#define BLKSTARTL (500) // ORIGINAL 500
#define BLKSTARTR (500) // ORIGINAL 500
#define BLACKRSP (500) // ORIGINAL 720
#define BLACKLSP (500) // ORIGINAL 720
#define BLACKR (600) // ORIGINAL 500
#define BLACKL (600) // ORIGINAL 500
#define BLACKDETECTR (500) // ORIGINAL 720
#define BLACKDETECTL (500) // ORIGINAL 720

#define ONESEC (5)
#define MS_COUNT (20)
#endif /* MACROS_H_ */



#endif /* MACROS_H_ */
