//===========================================================================
//  File Name : macros.h
//  Description:  This file contains the macros used by all the other files
//  Author: Connor Savugot
//  Date: Sep 12, 2024
//===========================================================================

#ifndef MACROS_H_
#define MACROS_H_
// General
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //
#define MCLK_FREQ_MHZ           (8) // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)

// State Machines
#define BLK_DETECTED ('D')
#define PARALLEL ('p')
#define FOLLOW ('F')
#define PAUSE_FOLLOW ('P')
#define INTERCEPT ('I')
#define IRCONF ('C')
#define SLOWRIGHT ('s')
#define WAIT ('W')
#define START ('S')
#define ARCH ('a')
#define EXIT ('E')
#define FORWARDS ('F')
#define FIND ('f')
#define BACK ('B')
#define PAD_MESSAGE ('M')

// Debounce
#define PRESSED (0)
#define RELEASED (1)
#define NOT_OKAY (0)
#define OKAY (1)
#define DEBOUNCE_RESTART (0)
#define DEBOUNCE_TIME (100)

//// Black Line Initial Command State Durations
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

//// Black Line Values
#define LOW_BLACK_LEFT (500) // ORIGINAL 500
#define LOW_BLACK_RIGHT (500) // ORIGINAL 500
#define HIGH_BLACK_LEFT (500) // ORIGINAL 720
#define HIGH_BLACK_RIGHT (500) // ORIGINAL 720


#endif /* MACROS_H_ */
