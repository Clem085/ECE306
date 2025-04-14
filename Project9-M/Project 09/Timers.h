/*
 * Timers.h
 *
 *  Created on: Nov 21, 2024
 *      Author: Kayla Radu
 */

#ifndef TIMERS_H_
#define TIMERS_H_



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



#endif /* TIMERS_H_ */
