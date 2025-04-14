/*
 * timersB0.h
 *
 * Description: Header file for Timer B0 configurations and constants.
 * Defines timer intervals and function prototypes for delays.
 *
 * Created on: Jan, 2025
 * Author: Molana Torabi
 */

#ifndef TIMERSB0_H_
#define TIMERSB0_H_

// Timer intervals for CCR registers (used for timing delays)
#define TB0CCR0_INTERVAL (25000)  // Timer B0 CCR0 interval
#define TB0CCR1_INTERVAL (25000)  // Timer B0 CCR1 interval (debounce timing)
#define TB0CCR2_INTERVAL (25000)  // Timer B0 CCR2 interval

// Motor control timing constants
#define WHEEL_PERIOD (50005)  // Full period for wheel PWM
#define WHEEL_OFF (0)         // Wheel off state
#define SLOW (35000)          // Slow speed setting
#define FAST (50000)          // Fast speed setting

// Duty cycle percentages for PWM
#define PERCENT_100 (50000)  // 100% duty cycle
#define PERCENT_80 (45000)   // 80% duty cycle

void half_sec_delay(void);

#endif /* TIMERSB0_H_ */
