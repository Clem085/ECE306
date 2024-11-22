// Global variables for debounce
// SW1 debounce counter
volatile unsigned int debounce_count1 = 0;
// SW2 debounce counter
volatile unsigned int debounce_count2 = 0;

// Backlight toggle state
volatile unsigned char backlight_on = 1;

// SW1 debounce active flag
volatile unsigned char debounce_active = 0;

// Global flags
//extern volatile int switch1_pressed;  // Flag to track switch 1 press

// Increment for the debounce interval for the switches
#define increment 3277;
