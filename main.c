#include <stdio.h>
#include <stdlib.h>
#include "Ks0108pi.h"
#include "GaragePi.h"

#include "fonts/metric01.h"
#include "fonts/metric02.h"
#include "fonts/metric04.h"

#include "fonts/garageclosed.h"
#include "fonts/garageopen.h"
#include "fonts/garageclosing.h"
#include "fonts/garageopening.h"
#include "fonts/garageerror.h"

#include <math.h>
#include <time.h>
#define PI 3.14159265
#define GPIO5 5
#define GPIO6 6

char * getTime(void);

int main(int argc, char** argv) {
	
	// grab our LCD structure instantiated in Ks0108pi.c
	extern Ks0108pi garagePiLCD;

	// Initialize LCD structure and BCM2835 IO library
	if(initGaragePi() != 0) { return 1; }
	if(init(&garagePiLCD) != 0) { return 1; }
	
	int prog = 0;
	char string[256];
	uint8_t * images[] = { garageclosed, garageopen, garageclosing, garageopening, garageerror };
	char * doorStatus[5] = {"CLOSED", "OPEN", "OPENING", "CLOSING", "ERROR"};
	char * doorSensor[3] = {"No switch", "Closed switch", "Open switch"};
	uint8_t whichSwitch = 0;
	char * timeStr;
	
	// Set RPI GPIO 5/6 to be an input
  bcm2835_gpio_fsel(GPIO5, BCM2835_GPIO_FSEL_INPT);
  bcm2835_gpio_fsel(GPIO6, BCM2835_GPIO_FSEL_INPT);
  // And a rising edge detect enable
  bcm2835_gpio_ren(GPIO5);
	bcm2835_gpio_ren(GPIO6);
	
	while(1) {
		for(int i=0; i < ( sizeof(images) / sizeof(images[0]) ); i++) {
			
			do {
				// clear frame buffer
				clearBuffer(&garagePiLCD);
				
				// check our inputs
				if(bcm2835_gpio_eds_multi( (1 << GPIO5) | (1 << GPIO6) )) {
					// Determine which button was pushed and clear eds flag by setting to 1
					if(bcm2835_gpio_eds(GPIO5)) {
						whichSwitch = 1;
						bcm2835_gpio_set_eds(GPIO5);
					} else if (bcm2835_gpio_eds(GPIO6)) {
						whichSwitch = 2;
						bcm2835_gpio_set_eds(GPIO6);
					} else {
						whichSwitch = 0;
					}
				}
				writeString(0, 28, doorSensor[whichSwitch], metric01, &garagePiLCD);
				
				// Draw our current time
				timeStr = getTime();
				writeString(0, 57, timeStr, metric01, &garagePiLCD);
				
				// Draw garage door interface
				writeString(0, 0, "GARAGE", metric02, &garagePiLCD);
				writeString(0, 15, doorStatus[i], metric02, &garagePiLCD);
				writeChar(78, 0, 0x00, images[i], &garagePiLCD);
				
				// Draw progress bar
				writeString(0, 40, "Please wait...", metric01, &garagePiLCD);
				drawRect(0, 48, 62, 7, STYLE_BLACK_BORDER, &garagePiLCD);
				drawRect(2, 50, (60 * prog) / 100, 3, STYLE_BLACK_BG, &garagePiLCD);
				
				// push frame buffer to display
				syncBuffer(&garagePiLCD);
				
				// increment progress bar
				prog += 8;

				// delay for 5s progress bar 100% * 50ms = 5s
				wait(200);
				
			} while(prog < 100);
			
			//reset progress bar
			prog = 0;
		}
	}
	// must call to free our allocated memory (framebuffer & BCM2835)
	close_BCM(&garagePiLCD);
	return 0;
}

char * getTime() {
	time_t rawTime = time(NULL); 
	return asctime(localtime(&rawTime));
}

/*	
	sprintf(string,"%d / 1547 MB   2.7 MB/S",(prog*1574)/100);
	kspi->writeString(3,18,string,metric01);
*/
