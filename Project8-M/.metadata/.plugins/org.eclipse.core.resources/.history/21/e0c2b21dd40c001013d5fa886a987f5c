/*
 * macros.h
 *
 * Created on: Jan, 2025
 * Author: Molana Torabi
 */

#ifndef MACROS_H_
#define MACROS_H_

// Constant Definitions
#define ALWAYS                  (1)     // Always true (used for logic conditions)
#define RESET_STATE             (0)     // Reset state, used for initializing variables

// LED Definitions
#define RED_LED                 (0x01)  // Bit mask for Red LED (P1.0)
#define GRN_LED                 (0x40)  // Bit mask for Green LED (P6.6)

// Debugging and Testing
#define TEST_PROBE              (0x01)  // Test probe bit mask
#define TRUE                    (0x01)  // Boolean TRUE representation

// State Definitions for the System
#define NONE                    ('N')   // No action state
#define STRAIGHT                ('L')   // Moving straight (possibly Left?)
#define STRAIGHTFW              ('F')   // Moving straight forward
#define STRAIGHTBW              ('B')   // Moving straight backward
#define SPINCOUNTERCLK          ('G')   // Spinning counterclockwise
#define PROJECTFIVE             ('P')   // Project-specific state
#define SPINCLK                 ('C')   // Spinning clockwise
#define EMPTY                   ('E')   // Empty state
#define WAIT                    ('W')   // Waiting state
#define START                   ('S')   // Start state
#define DETECTED                ('D')   // Object detected state
#define SPIN                    ('O')   // Generic spin state
#define END                     ('E')   // End state
#define RUN                     ('R')   // Running state
#define WAIT                    ('W')   // Waiting state (duplicate, consider removing)
#define START                   ('S')   // Start state (duplicate, consider removing)
#define DETECTED                ('D')   // Object detected state (duplicate, consider removing)
#define SPIN                    ('O')   // Generic spin state (duplicate, consider removing)
#define TRACK                   ('T')   // Tracking state (e.g., following a line)
#define TRACKWAIT               ('K')   // Waiting during tracking
#define TURNSTOP                ('2')   // Stop during a turn
#define TURNN                   ('5')   // Turn state (specific to project logic)
#define STRAIGHTL               ('!')   // Special straight state (specific to project logic)
#define STRAIGHTSTOP            ('8')   // Stop during straight movement

// Port Register Aliases
#define P4PUD                   (P4OUT) // Port 4 pull-up/down configuration alias
#define P2PUD                   (P2OUT) // Port 2 pull-up/down configuration alias

// Timer Interrupt Vectors
#define TIMER_B0_0_VECTOR       (TIMER0_B0_VECTOR)  // Timer B0 interrupt vector (low priority)
#define TIMER_B0_1_OVFL_VECTOR  (TIMER0_B1_VECTOR)  // Timer B0 overflow/compare vector (high priority)

// PWM Control Macros
#define PWM_PERIOD              (TB3CCR0)  // PWM period register (Timer B3)
#define LCD_BACKLITE_DIMING     (TB3CCR1)  // LCD backlight dimming control (Timer B3 CCR1)
#define WHEEL_OFF               (0)        // Wheels off (stopping motor movement)

// Motor Speed Control
#define SLOW                    (20000)   // Slow motor speed (PWM duty cycle value)
#define FAST                    (50000)   // Fast motor speed (PWM duty cycle value)
#define PERCENT_100             (50000)   // 100% duty cycle for PWM (full speed)
#define PERCENT_80              (45000)   // 80% duty cycle for PWM (reduced speed)

#endif /* MACROS_H_ */
