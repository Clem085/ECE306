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


// States
#define IDLE                    ('I')
#define WAIT                    ('W')
#define FWD                     ('F')
#define BLACKLINE               ('B')
#define WAIT2                   ('w')
#define TURNL                   ('T')
#define LINE1                   ('L')
#define LINE2                   ('l')
#define DONE                    ('D')

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

#define IR_MAGIC_NUM (300)


#define HALF_SEC (500)
#define COUNTER_RESET (0)


#endif /* MACROS_H_ */
