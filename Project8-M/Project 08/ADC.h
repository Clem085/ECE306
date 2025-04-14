/*
 * ADC.h
 * Analog-to-Digital Converter (ADC) Interface
 *
 * Created on: Mar 2, 2025
 * Author: torab
 */

#ifndef ADC_H_
#define ADC_H_

// Global Variables
//// Internally Defined
volatile unsigned int ADC_Channel;      // Currently selected ADC channel
volatile unsigned int ADC_Left_Detect;  // Raw value from left IR detector (A2)
volatile unsigned int ADC_Right_Detect; // Raw value from right IR detector (A3)
volatile unsigned int ADC_Thumb;        // Raw value from thumbwheel potentiometer (A5)
int ADC_Prev;                           // Previous ADC reading for comparison
int ADC_Temp;                           // Temporary storage for ADC calculations
char ADC_Changed;                       // Flag indicating ADC value change
char ADC_Update;                        // Flag indicating new ADC data available
char ADC_Display;                       // Flag controlling display updates
char adc_char[10];                      // Buffer for ASCII-converted ADC values

//// Externally Defined
// (Variables from other modules would be declared here with 'extern')

// Macro Definitions
// (ADC-related constants would be defined here)

// Function Declarations
/**
 * Initializes ADC hardware and configures:
 * - Input channels (A2, A3, A5)
 * - Conversion parameters (12-bit, single-channel mode)
 * - Interrupts
 */
void Init_ADC(void);

/**
 * Converts a 16-bit hexadecimal value to 4-digit BCD format
 * @param hex_value The raw ADC value to convert
 * Result stored in adc_char[] as ASCII string
 */
void HexToBCD(int hex_value);

// Deprecated functions (commented out but kept for reference)
//void ADC_Process(void);  // Old ADC data handler
//void ADC_control(void);  // Old display control function

#endif /* ADC_H_ */
