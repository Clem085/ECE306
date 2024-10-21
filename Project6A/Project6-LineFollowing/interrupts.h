/*
 * interrupts.h
 *
 *  Created on: Oct 21, 2024
 *      Author: jammi
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_


// Globals

// Defines
// PWM
// REMEMBER
    // Reverse or Forward must be 0 when the other is not 0.
    // DESIRED ON AMOUNT must be a value less than WHEEL_PERIOD
    // If WHEEL_PERIOD is 50,005 then
    //DESIRED ON AMOUNT of 20,000 would be ON NEAR 0%
#define WHEEL_PERIOD    (50005)
#define WHEEL_OFF       (0)
#define SLOW            (35000)
#define FAST            (50000)
#define PERCENT_100     (50000)
#define PERCENT_80      (45000)

#endif /* INTERRUPTS_H_ */
