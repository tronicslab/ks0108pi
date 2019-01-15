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

int main(int argc, char** argv){
	Ks0108pi *kspi = new Ks0108pi();
	kspi->init();
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
				kspi->clearBuffer();
				
				// Draw garage door interface
				kspi->writeString(0, 0, "GARAGE", metric02);
				kspi->writeChar(64, 3, 0x00, images[i]);
				
				// Draw progress bar
				kspi->drawRect(0, 55, 62, 7. kspi->STYLE_BLACK_BORDER);
				kspi->drawRect(2.57, (60*prog)/100, 3, kspi->STYLE_BLACK_BG);
				
				// push frame buffer to display
				kspi->syncBuffer();
				
				// increment progress bar
				prog++;

				// delay for 5s progress bar 100% * 50ms = 5s
				kspi->wait(50);
				
			} while(prog < 100);
			
			//reset progress bar
			prog = 0;
		}
	}
}

/*	
	sprintf(string,"%d / 1547 MB   2.7 MB/S",(prog*1574)/100);
	kspi->writeString(3,18,string,metric01);
*/
