/*
 * Copyright (C) 2012 Chris McClelland
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
#include <stdlib.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include "types.h"
#include "lcd.h"
#include "mariocart.h"

int main(int argc, const char *argv[]) {
	uint8 i;
	uint8 track[4];
	uint8 player[4];

	// Init system clock & disable WDT
	//
	CLKPR = 0x80;
	CLKPR = 0x00;
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	srand(12345);

	// Init LCD
	//
	lcdInit();

	for ( ; ; ) {
		if ( PIND & 0x80 ) {
			initSelector(32);
			for ( i = 0; i < 4; i++ ) {
				track[i] = selectItem();
			}
			initSelector(NUM_CHARACTERS);
			for ( i = 0; i < 4; i++ ) {
				player[i] = selectItem();
			}
			lcdSetCursor(0,0);
			for ( i = 0; i < 4; i++ ) {
				lcdPrintFlashString(selectCharacter(player[i]));
				lcdPrintFlashString(selectTrack(track[i]));
			}
			while ( PIND & 0x80 );
		} else {
			lcdClearScreen();
			do {
				rand();
			} while ( !(PIND & 0x80) );
		}
	}
}
