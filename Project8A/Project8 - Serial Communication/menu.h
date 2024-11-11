//===========================================================================
//  File Name: menu.h
//  Description: Header file for menu.c - Contains Globals, Macros, & function declarations
//  Programmer: Connor Savugot
//  Date Created: Nov 10, 2024
//===========================================================================

#ifndef MENU_H_
#define MENU_H_

// Global Variables
//// Internally Defined
char menu;
//// Externally Defined
extern volatile unsigned int ADC_Thumb;


// Macro Definitions
//// Menu States
#define IDLE        ('I')
#define RESISTOR    ('R')
#define SHAPES      ('S')
#define SONG        ('s')
//// Colors
#define BLACK       (0)
#define BROWN       (1)
#define RED         (2)
#define ORANGE      (3)
#define YELLOW      (4)
#define GREEN       (5)
#define BLUE        (6)
#define VIOLET      (7)
#define GRAY        (8)
#define WHITE       (9)


// Function Declarations
void menuSelect(void);
void menuScroll(void);

#endif /* MENU_H_ */
