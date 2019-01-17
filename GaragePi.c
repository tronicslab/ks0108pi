/*
	Raspberry pi garage door controller
	Author: Ryan MacIntosh

	With help from the following amazing libraries :D :
		- BCM 2835 library by Mike McCauley
*/

#include "GaragePi.h"

int initGaragePi() {
	if (!bcm2835_init()){
		printf("Failed to load bcm2835...\n");
		return 1;
	} else {
		printf("Initialize bcm2835...success!\n");	
	}

	return 0;
}
