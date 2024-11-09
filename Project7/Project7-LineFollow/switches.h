#ifndef SWITCHES_H_
#define SWITCHES_H_

void Switch1_Proj5_Process(void);

// Enable Switches
void enable_switch_SW1(void);
void enable_switch_SW2(void);
void enable_switches(void);

// Disable Switches
void disable_switch_SW1(void);
void disable_switch_SW2(void);
void disable_switches(void);

// Debounce
void debounce(void);
void debounceSW1(void);
void debounceSW2(void);

// DEFINES
#define DEBOUNCE_TIME          (6)
#define DEBOUNCE_RESTART       (0)

#endif /* SWITCHES_H_ */
