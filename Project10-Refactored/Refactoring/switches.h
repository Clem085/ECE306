//===========================================================================
//  File Name: switches.h
//  Description: Header file for switches.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Sep 20, 2024
//===========================================================================
#define SWITCHES_H_

// Global Variables
char dispEvent;
unsigned int straight_step;
unsigned int circle_step;
unsigned int circle_step2;
unsigned int triangle_step;
unsigned int figure8_step;
unsigned int SW1_Okay;
unsigned int debounce_Status_SW1;
unsigned int count_debounce_SW1;
unsigned int SW2_Okay;
unsigned int debounce_Status_SW2;
unsigned int count_debounce_SW2;
unsigned int shapes_count;
extern char display_line[4][11];
unsigned int event;
unsigned char display_changed;

// Macros
//#define DEBOUNCE_RESTART          (0x00) //
//#define DEBOUNCE_TIME             (0x05) //
//#define NOT_OKAY                  (0x00) //
//#define OKAY                      (0x01) //
#define OFF                       (0x00) //
#define ON                        (0x01) //
#define RESTART                   (0x00) //
#define STRAIGHT                  ('1')
//#define CIRCLE                    ('2')
//#define TRIANGLE                  ('3')
#define FIGURE8C1                 ('4')
//#define NONE                      ('5')


// Functions
void debounceSW1(void);
void debounceSW2(void);
void Project8_SW1(void);
void Project5_SW1(void);
