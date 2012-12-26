/*
 * Copyright (C) 2009 Chris McClelland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LCD_H
#define LCD_H

#include "types.h"

// Initialise the LCD according to the HD44780 datasheet.
//
void lcdInit(void);

// Clear the screen, return the cursor to the top left.
//
void lcdClearScreen(void);

// Set the cursor position to the given coordinates without affecting the screen.
//
void lcdSetCursor(uint8 x, uint8 y);

// Replace the character at the current cursor position with the supplied character, except where
// it is 0x0A, in which case the LCD gets a linefeed.
//
void lcdPrintChar(char ch);

// Write the supplied NUL-terminated string to the current cursor position.
//
void lcdPrintString(const char *str);

// Write the supplied NUL-terminated PROGMEM string to the current cursor position.
//
void lcdPrintFlashString(const char *str);

#endif
