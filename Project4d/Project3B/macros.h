/*
 * File Name: macros.h
 *  Description:
 *  Created on: Sep 12, 2024
 *  Author: Connor Savugot
 */

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
#define WAIT                  ('W')
#define START                 ('S')
#define RUN
('R')
#define END                   ('E')
#define WHEEL_COUNT_TIME       (10)
#define RIGHT_COUNT_TIME        (7)
#define LEFT_COUNT_TIME         (8)
#define TRAVEL_DISTANCE         (2)
#define WAITING2START          (50)

#endif /* MACROS_H_ */
