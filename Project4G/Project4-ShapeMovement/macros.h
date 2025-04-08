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
#define NONE                  ('N')
#define STRAIGHT              ('L')
#define CIRCLE                ('C')
#define TRIANGLE              ('T')
#define TRIANGLE_CURVE        ('t')
#define FIGURE8C1             ('F')
#define FIGURE8C2             ('f')
#define WAIT                  ('W')
#define START                 ('S')
#define RUN                   ('R')
#define END                   ('E')

// Switches
#define PRESSED                (0)
#define RELEASED               (1)
#define NOT_OKAY               (0)
#define OKAY                   (1)
#define DEBOUNCE_TIME          (12)
#define DEBOUNCE_RESTART       (0)
#define WAITING2START          (500) //1000 Long


#endif /* MACROS_H_ */
