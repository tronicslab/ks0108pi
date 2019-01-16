/*
	Raspberry pi garage door controller
	Author: Ryan MacIntosh

	With help from the following amazing libraries :D :
		- BCM 2835 library by Mike McCauley
*/

#include "GaragePi.h"

int initGPIOlib() {
	if (!bcm2835_init()){
		printf("Failed to load bcm2835...\n");
		return 1;
	}

	printf("Started bcm2835...\n");
	return 0;
}
