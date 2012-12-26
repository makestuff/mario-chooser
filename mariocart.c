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
#include <avr/pgmspace.h>
#include "mariocart.h"
#include "rand.h"
#include "types.h"

static const char c00[] PROGMEM = "Baby Mario    ";
static const char c01[] PROGMEM = "Baby Peach    ";
static const char c02[] PROGMEM = "Birdo         ";
static const char c03[] PROGMEM = "Bowser        ";
static const char c04[] PROGMEM = "Diddy Kong    ";
static const char c05[] PROGMEM = "Donkey Kong   ";
static const char c06[] PROGMEM = "Daisy         ";
static const char c07[] PROGMEM = "Dry Bones     ";
static const char c08[] PROGMEM = "King Boo      ";
static const char c09[] PROGMEM = "Koopa Troopa  ";
static const char c10[] PROGMEM = "Luigi         ";
static const char c11[] PROGMEM = "Mario         ";
static const char c12[] PROGMEM = "Peach         ";
static const char c13[] PROGMEM = "Rosalina      ";
static const char c14[] PROGMEM = "Toad          ";
static const char c15[] PROGMEM = "Toadette      ";
static const char c16[] PROGMEM = "Waluigi       ";
static const char c17[] PROGMEM = "Wario         ";
static const char c18[] PROGMEM = "Yoshi         ";

static const char t00[] PROGMEM = " | 1:1";
static const char t01[] PROGMEM = " | 1:2";
static const char t02[] PROGMEM = " | 1:3";
static const char t03[] PROGMEM = " | 1:4";
static const char t04[] PROGMEM = " | 2:1";
static const char t05[] PROGMEM = " | 2:2";
static const char t06[] PROGMEM = " | 2:3";
static const char t07[] PROGMEM = " | 2:4";
static const char t08[] PROGMEM = " | 3:1";
static const char t09[] PROGMEM = " | 3:2";
static const char t10[] PROGMEM = " | 3:3";
static const char t11[] PROGMEM = " | 3:4";
static const char t12[] PROGMEM = " | 4:1";
static const char t13[] PROGMEM = " | 4:2";
static const char t14[] PROGMEM = " | 4:3";
static const char t15[] PROGMEM = " | 4:4";
static const char t16[] PROGMEM = " | 5:1";
static const char t17[] PROGMEM = " | 5:2";
static const char t18[] PROGMEM = " | 5:3";
static const char t19[] PROGMEM = " | 5:4";
static const char t20[] PROGMEM = " | 6:1";
static const char t21[] PROGMEM = " | 6:2";
static const char t22[] PROGMEM = " | 6:3";
static const char t23[] PROGMEM = " | 6:4";
static const char t24[] PROGMEM = " | 7:1";
static const char t25[] PROGMEM = " | 7:2";
static const char t26[] PROGMEM = " | 7:3";
static const char t27[] PROGMEM = " | 7:4";
static const char t28[] PROGMEM = " | 8:1";
static const char t29[] PROGMEM = " | 8:2";
static const char t30[] PROGMEM = " | 8:3";
static const char t31[] PROGMEM = " | 8:4";

static const PGM_P characterList[] PROGMEM = {
	c00, c01, c02, c03, c04, c05, c06, c07, c08, c09,
	c10, c11, c12, c13, c14, c15, c16, c17, c18
};

static const PGM_P trackList[] PROGMEM = {
	t00, t01, t02, t03, t04, t05, t06, t07, t08, t09,
	t10, t11, t12, t13, t14, t15, t16, t17, t18, t19,
	t20, t21, t22, t23, t24, t25, t26, t27, t28, t29,
	t30, t31
};

static uint8 array[32];
static uint8 numItems;

void initSelector(const uint8 size) {
	uint8 i;
	numItems = size;
	for ( i = 0; i < numItems; i++ ) {
		array[i] = i;
	}
}

uint8 selectItem(void) {
	const uint8 limit = numItems - 1;
	const uint8 item = getRandom(numItems);
	const uint8 result = array[item];
	uint8 i;
	for ( i = item; i < limit; i++ ) {
		array[i] = array[i + 1];
	}
	numItems = limit;
	return result;
}

const char *selectTrack(const uint8 tk) {
	return (const char *)pgm_read_word(&trackList[tk]);
}

const char *selectCharacter(const uint8 ch) {
	return (const char *)pgm_read_word(&characterList[ch]);
}

/*
int main(void) {
	const char *track[4];
	const char *player[4];
	uint8 i;
	seedRandom(0);

	//for ( j = 0; j < 1000000; j++ ) {
		initSelector(16);
		for ( i = 0; i < 4; i++ ) {
			track[i] = trackList[selectItem()];
		}
		initSelector(NUM_CHARACTERS);
		for ( i = 0; i < 4; i++ ) {
			player[i] = characterList[selectItem()];
		}
	//	printf("%s\n", player[3]);
	//}
	for ( i = 0; i < 4; i++ ) {
		printf("  %s%s\n", player[i], track[i]);
	}
	
	return 0;
}
*/
