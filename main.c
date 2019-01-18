#include <stdio.h>
#include <stdlib.h>
#include "Ks0108pi.h"
#include "GaragePi.h"
//
#include "fonts/metric01.h"
#include "fonts/metric02.h"
#include "fonts/metric04.h"
//
#include "fonts/pbar.h"
#include "fonts/batticon.h"
#include "fonts/garageclosed.h"
#include "fonts/garageopen.h"
#include "fonts/garageclosing.h"
#include "fonts/garageopening.h"
#include "fonts/garageerror.h"

#include <math.h>
#define PI 3.14159265

int main(int argc, char** argv) {
	
	// grab our LCD structure instantiated in Ks0108pi.c
	extern Ks0108pi garagePiLCD;

	// Initialize LCD structure and BCM2835 IO library
	if(init(&garagePiLCD) != 0) { return 1; }
	if(initGaragePi() != 0) { return 1; }
	
	int prog = 0;
	char string[256];
	uint8_t * images[] = { 
													garageclosed, 
													garageopen, 
													garageclosing, 
													garageopening, 
													garageerror
												};
	
	while(1) {
		for(int i=0; i < ( sizeof(images) / sizeof(images[0]) ); i++) {
			
			do {
				// clear frame buffer
				clearBuffer(&garagePiLCD);
				
				// Draw garage door interface
				writeString(0, 0, "GARAGE", metric02, &garagePiLCD);
				writeChar(64, 3, 0x00, images[i], &garagePiLCD);
				
				// Draw progress bar
				drawRect(0, 55, 62, 7, STYLE_BLACK_BORDER, &garagePiLCD);
				drawRect(2, 57, (60 * prog) / 100, 3, STYLE_BLACK_BG, &garagePiLCD);
				
				// push frame buffer to display
				syncBuffer(&garagePiLCD);
				
				// increment progress bar
				prog++;

				// delay for 5s progress bar 100% * 50ms = 5s
				wait(50);
				
			} while(prog < 100);
			
			//reset progress bar
			prog = 0;
		}
	}
	close(&garagePiLCD);
	return 0;
}

/*	
	sprintf(string,"%d / 1547 MB   2.7 MB/S",(prog*1574)/100);
	kspi->writeString(3,18,string,metric01);
*/
