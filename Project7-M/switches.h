/*
 * switches.h
 *
 *
 * Created on: Jan, 2025
 * Author: Molana Torabi
 */

#define SWITCHES_H_

// Define debounce constants for managing switch input stability
#define DEBOUNCE_RESTART          (0x00)  // State for restarting debounce counter
#define DEBOUNCE_TIME             (0x05)  // Time threshold for debounce in units (e.g., ticks)
#define NOT_OKAY                  (0x00)  // State indicating that the switch input is not valid or not okay
#define OKAY                      (0x01)  // State indicating that the switch input is valid and okay
#define OFF                       (0x00)  // State for switch being OFF
#define ON                        (0x01)  // State for switch being ON
#define RESTART                   (0x00)  // State indicating a restart condition for the system or process
