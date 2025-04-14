/* IR Program Information
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
  File Name : IR.h
  Description:  Header file for Infrared (IR) LED control
                - Defines IR LED control interface
                - Declares status variables and constants
  Programmer: Molana
  Date Created: March, 2025
-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
*/

#ifndef IR_H_
#define IR_H_

// Global Variables
//// Internally Defined
char IR_status;    // Controls IR LED state:
                   // - Set to OFF (0) to turn IR LED off
                   // - Set to ON (1) to turn IR LED on
char IR_changed;   // Flag indicating IR status change (used for debouncing/notification)

//// Externally Defined
// (Variables from other modules would be declared here with 'extern')

// Macro Definitions
#define IR_MAGIC_NUM (500)  // Special constant used for IR timing/calibration
                            // Represents threshold value for IR detection logic

// Function Declarations
/**
 * Controls the physical IR LED state
 * - Reads global IR_status variable
 * - Directly manipulates hardware port pins
 * - Should be called periodically to maintain desired state
 */
void IR_control(void);

#endif /* IR_H_ */
