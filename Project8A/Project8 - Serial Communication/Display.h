#ifndef DISPLAY_H_
#define DISPLAY_H_


// Globals
char backlight_status;
char light_percent;
int middleChar(char * string);

// Functions
void backlight_control(void);
void dispPrint(char *line, char lineToUpdate);
void adc_line(char line, char location);
void PWM_backlight(void);

// DEFINES
// PWM
#define LCD_BACKLITE_DIMING (TB3CCR1)
#define PERCENT_100 (50000)
#define PERCENT_80  (45000)


#endif /* DISPLAY_H_ */
