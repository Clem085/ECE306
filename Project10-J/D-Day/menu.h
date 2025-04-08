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
char menuType;
int ADC_Outer;
int ADC_Inner;
unsigned int i;
unsigned int j;
char songInit;

//// Externally Defined
extern unsigned int ADC_Thumb;
extern unsigned char display_changed;
extern char display_line[4][11];

// Macro Definitions
//// Menu Types
#define OUTER_MENU  ('M')
#define INNER_MENU  ('m')
#define IDLE        ('I')
//// Outer Menu States
#define RESISTOR    ('R')
#define SHAPES      ('S')
#define SONG        ('s')
//// Inner Menu States
////// Colors
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
////// Shapes
#define CIRCLE      (0)
#define SQUARE      (1)
#define TRIANGLE    (2)
#define OCTAGON     (3)
#define PENTAGON    (4)
#define HEXAGON     (5)
#define CUBE        (6)
#define OVAL        (7)
#define SPHERE      (8)
#define CYLINDER    (9)

// Function Declarations
void menuScroll(void);
void ResistorMenu(void);
void ShapeMenu(void);
void SongMenu(void);
#endif /* MENU_H_ */
