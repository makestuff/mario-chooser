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
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "types.h"
#include "delay.h"
#include "lcd.h"

// Comment this out to disable scrolling; this will cause text to wrap around to the top of the
// screen instead of the screen itself scrolling upwards. Disabling scrolling will save 61 bytes
// of RAM and about 450 bytes of flash.
//
//#define SCROLLING

// These assume you have connected the LCD's EN to PC2, the LCD's RS to PD0, and the LCD's DB4..7
// to PC4..7. Replace these with your own implementations if you're connecting the LCD to other
// port pins.
//
static inline void writeTopNibbleAndAssertEnable(uint8 byte) {
	PORTC &= 0x0F;  // Clear top four bits; leave others unchanged
	PORTC |= (byte & 0xF0) | 0x04;  // Write top four bits and set EN high
}
static inline void clearEnable(void) {
	PORTC &= 0xFB;  // Clear enable
}
static inline void initPorts(void) {
	DDRC |= 0xF4;  // PC2 (EN), PC4..7 outputs
	DDRD |= 0x01;  // PD0 (RS) output
}
static inline void selectDataRegister(void) {
	PORTD |= 0x01;
}
static inline void selectControlRegister(void) {
	PORTD &= 0xFE;
}

// State needed for scrolling the screen...uses 61 bytes of RAM
//
#ifdef SCROLLING
	typedef struct {
		unsigned int offset : 6;
		unsigned int needScroll : 1;
		unsigned int needRemember : 1;
	} Status;
	static Status status = {0, 0, 0};
	static char screen[60];
#endif

// Keep track of the LCD's DDRAM address
//
static uint8 address;

// "Private" functions
//
static void writeByte(uint8 byte) {
	writeTopNibbleAndAssertEnable(byte);
	delay(2);
	clearEnable();
	delay(2);
	writeTopNibbleAndAssertEnable(byte << 4);
	delay(2);
	clearEnable();
	delay(2);
}

static void writeNibble(uint8 byte) {
	writeTopNibbleAndAssertEnable(byte << 4);
	delay(2);
	clearEnable();
	delay(2);
}

static void setAddress(void) {
	selectControlRegister();
	writeByte(address | 0x80);
	delay(50*4);
	selectDataRegister();
}	

static inline void gotoLine0(uint8 x) {
	address = x;
	setAddress();
	#ifdef SCROLLING
		status.offset = 0;
		status.needScroll = 0;
		status.needRemember = 0;
	#endif
}

static inline void gotoLine1(uint8 x) {
	address = 0x40 + x;
	setAddress();
	#ifdef SCROLLING
		status.offset = x;
		status.needScroll = 0;
		status.needRemember = 1;
	#endif
}

static inline void gotoLine2(uint8 x) {
	address = 0x14 + x;
	setAddress();
	#ifdef SCROLLING
		status.offset = 20 + x;
		status.needScroll = 0;
		status.needRemember = 1;
	#endif
}

static inline void gotoLine3(uint8 x) {
	address = 0x54 + x;
	setAddress();
	#ifdef SCROLLING
		status.offset = 40 + x;
		status.needScroll = 0;
		status.needRemember = 1;
	#endif
}

#ifdef SCROLLING
	// The screen buffer always contains a copy of what is on lines 2,3,4 of the screen.
	//
	static void doScroll(void) {
		uint8 i;
		char *dst = screen;
		const char *src = screen + 20;

		// Write what was on the 2nd line on the 1st line
		//
		address = 0x00;
		setAddress();
		for ( i = 0; i < 20; i++ ) {
			writeByte((uint8)screen[i]);
			delay(41*4);
		}

		// Now copy the 3rd and 4th line to the 2nd and 3rd
		//
		for ( i = 0; i < 40; i++ ) {
			*dst++ = *src++;
		}

		// Now clear the last line
		//
		for ( i = 0; i < 20; i++ ) {
			*dst++ = ' ';
		}

		// Now fill in lines 2,3,4
		//
		for ( i = 20; i < 40; i++ ) {
			writeByte((uint8)screen[i]);
			delay(41*4);
		}
		for ( i = 0; i < 20; i++ ) {
			writeByte((uint8)screen[i]);
			delay(41*4);
		}
		for ( i = 40; i < 60; i++ ) {
			writeByte((uint8)screen[i]);
			delay(41*4);
		}
		status.offset = 40;
		address = 0x54;
		setAddress();
	}
#endif

// Public API
//
void lcdInit(void) {
	#ifdef SCROLLING
		uint8 i;
	#endif
	initPorts();
	delay(60000);
	selectControlRegister();
	writeNibble(0x03);
	delay(4100*4);
	writeNibble(0x03);
	delay(100*4);
	writeNibble(0x03);
	delay(37*4);
	writeNibble(0x02); // select 4-bit mode
	writeByte(0x28);   // 4-bit mode, 2-line display, 5x8 font.
	delay(37*4);
	writeByte(0x0C);   // Display on
	delay(37*4);
	writeByte(0x01);   // Display clear
	delay(37*4);
	writeByte(0x06);   // Entry mode set
	delay(37*4);
	writeByte(0x02);   // Return home
	delay(1600*4);
	address = 0;
	#ifdef SCROLLING
		status.offset = 0;
		status.needScroll = 0;
		status.needRemember = 0;
		for ( i = 0; i < 60; i++ ) {
			screen[i] = ' ';
		}
	#endif
	selectDataRegister();
}

void lcdClearScreen(void) {
	#ifdef SCROLLING
		uint8 i;
	#endif
	selectControlRegister();
	writeByte(0x01);   // Display clear
	delay(37*4);
	writeByte(0x02);   // Return home
	delay(1600*4);
	address = 0;
	#ifdef SCROLLING
		status.offset = 0;
		status.needScroll = 0;
		status.needRemember = 0;
		for ( i = 0; i < 60; i++ ) {
			screen[i] = ' ';
		}
	#endif
	selectDataRegister();
}

void lcdSetCursor(uint8 x, uint8 y) {
	y &= 0x03;
	#ifdef SCROLLING
		status.needScroll = 0;
	#endif
	switch ( y ) {
		case 0:
			gotoLine0(x);
			break;
		case 1:
			gotoLine1(x);
			break;
		case 2:
			gotoLine2(x);
			setAddress();
			break;
		case 3:
			gotoLine3(x);
			break;
	}
}

void lcdPrintChar(char ch) {
	#ifdef SCROLLING
		if ( status.needScroll ) {
			doScroll();
		}
		status.needScroll = 0;
	#endif
	if ( ch == 0x0A ) {
		if ( address >= 0x00 && address <= 0x13 ) {
			gotoLine1(0);
		} else if ( address >= 0x40 && address <= 0x53 ) {
			gotoLine2(0);
		} else if ( address >= 0x14 && address <= 0x27 ) {
			gotoLine3(0);
		} else if ( address >= 0x54 && address <= 0x67 ) {
			#ifdef SCROLLING
				status.needScroll = 1;
			#else
				gotoLine0(0);
			#endif
		}
	} else {
		writeByte((uint8)ch);
		delay(41*4);
		#ifdef SCROLLING
			if ( status.needRemember ) {
				screen[status.offset] = ch;
			}
		#endif
		if ( address == 0x13 ) {
			// We're at the end of the first line
			//
			address = 0x40;
			setAddress();
			#ifdef SCROLLING
				status.offset = 0;
				status.needScroll = 0;
				status.needRemember = 1;  // Need to start remembering
			#endif
		} else if ( address == 0x53 ) {
			// We're at the end of the second line
			//
			address = 0x14;
			setAddress();
			#ifdef SCROLLING
				status.offset = 20;
			#endif
		} else if ( address == 0x27 ) {
			// We're at the end of the third line
			//
			address = 0x54;
			setAddress();
			#ifdef SCROLLING
				status.offset = 40;
			#endif
		} else if ( address == 0x67 ) {
			// We're at the end of the fourth line
			//
			#ifdef SCROLLING
				status.needScroll = 1;
			#else
				address = 0x00;
				setAddress();
			#endif
		} else {
			address++;
			#ifdef SCROLLING
				status.offset++;
			#endif
		}
	}
}

void lcdPrintFlashString(const char *str) {
	char ch = pgm_read_byte(str);
	while ( ch ) {
		lcdPrintChar(ch);
		str++;
		ch = pgm_read_byte(str);
	}
}

void lcdPrintString(const char *str) {
	while ( *str ) {
		lcdPrintChar(*str);
		str++;
	}
}
