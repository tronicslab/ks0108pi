/*
	Raspberry pi garage door controller
	Author: Ryan MacIntosh

	With help from the following amazing libraries :D :
		- BCM 2835 library by Mike McCauley
*/

#include "GaragePi.h"
#include "Ks0108pi.h"

Ks0108pi garagePiLCD = {	// then use extern Ks0108pi garagePiLCD;
	0x40, 0xB8, 0xC0, 		// DISPLAY_SET_Y, DISPLAY_SET_X, DISPLAY_START_LINE, 
	0x3E, 0x01, 0x00, 		// DISPLAY_ON_CMD, ON, OFF,
	128, 64 							// SCREEN_WIDTH, SCREEN_HEIGHT
};

int initGaragePi() {
	if (!bcm2835_init()){
		printf("Failed to load bcm2835...\n");
		return 1;
	} else {
		printf("Initialize bcm2835...success!\n");	
	}

	return 0;
}
