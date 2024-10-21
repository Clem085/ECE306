#ifndef TIMERSB0_H_
#define TIMERSB0_H_

void half_sec_delay(void);

// TIMERS
    // Timer A0
#define TB0CCR0_INTERVAL (25000) // 8,000,000 / 8 / 8 / (1000 / 200msec)
    // Others
#define TB0CCR1_INTERVAL (25000) // 8,000,000 / 8 / 8 / (1000 / 200msec)
#define TB0CCR2_INTERVAL (25000) // 8,000,000 / 8 / 8 / (1000 / 200msec)

    // Timer B3
#define TB3CCR0_INTERVAL (5000)


#define TIMER_B0_0_VECTOR (TIMER0_B0_VECTOR)
#define TIMER_B0_1_OVFL_VECTOR (TIMER0_B1_VECTOR)

#define TIMER_B3_0_VECTOR (TIMER0_B3_VECTOR)

#endif /* TIMERSB0_H_ */
