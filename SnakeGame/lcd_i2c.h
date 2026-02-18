#ifndef LCD_I2C_H
#define LCD_I2C_H

#include "MKL05Z4.h"

// Funkcje dostepne dla innych plików
void LCD_Init(void);
void LCD_Clear(void);
void LCD_SetCursor(int x, int y);
void LCD_Char(char c);
void LCD_String(char* str);

#endif