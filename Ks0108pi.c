/*
	Raspberry pi KS0108 LCD interface
	Author: Christian Isenberg

	With help from the following amazing libraries :D :
		- Universal KS0108 driver library by  Rados³aw Kwiecieñ, radek@dxp.pl
		- BCM 2835 library by Mike McCauley
*/

#include "Ks0108pi.h"

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
int init(Ks0108pi* myLCD) {

	// LCD pins
	myLCD->PIN_RS  = 7;
	myLCD->PIN_EN  = 11;
	myLCD->PIN_CS1 = 25;
	myLCD->PIN_CS2 = 8;
	myLCD->PIN_D0  = 2;
	myLCD->PIN_D1  = 3;
	myLCD->PIN_D2  = 4;
	myLCD->PIN_D3  = 17;
	myLCD->PIN_D4  = 27;
	myLCD->PIN_D5  = 22;
	myLCD->PIN_D6  = 10;
	myLCD->PIN_D7  = 9;
	
	// other
	myLCD->screen_x = 0;
	myLCD->screen_y = 0;
	
	//sets all pins as output
	bcm2835_gpio_fsel(myLCD->PIN_EN,  BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(myLCD->PIN_RS,  BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(myLCD->PIN_CS1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(myLCD->PIN_CS2, BCM2835_GPIO_FSEL_OUTP);

	bcm2835_gpio_fsel(myLCD->PIN_D0, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(myLCD->PIN_D1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(myLCD->PIN_D2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(myLCD->PIN_D3, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(myLCD->PIN_D4, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(myLCD->PIN_D5, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(myLCD->PIN_D6, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(myLCD->PIN_D7, BCM2835_GPIO_FSEL_OUTP);

	// initialize controllers
	for(int i = 0; i < 2; i++)
		writeCommand((DISPLAY_ON_CMD | ON), i);

	// initialize frame buffer and clearout with 0's
	myLCD->framebuffer_size = (myLCD->SCREEN_WIDTH * myLCD->SCREEN_HEIGHT) / 8;
	//myLCD.framebuffer = new uint8_t[framebuffer_size];			// new? not c?
	myLCD->framebuffer = malloc(myLCD->framebuffer_size * sizeof(uint8_t));	// remember!! free(myLCD.framebuffer)
  if(myLCD->framebuffer == NULL) { 
  	printf("Failed to create framebuffer!\n"); 
  	return 1; 
  }
	//std::fill_n(framebuffer,framebuffer_size, 0);
	memset(myLCD->framebuffer, 0, myLCD->framebuffer_size);
  return 0; // all good
}

//-------------------------------------------------------------------------------------------------
//	Free our allocated memory for the framebuffer
//-------------------------------------------------------------------------------------------------
void close(Ks0108pi* myLCD) {
	free(myLCD->framebuffer);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void goTo(uint8_t x, uint8_t y, Ks0108pi* myLCD) {
	uint8_t i;
	myLCD->screen_x = x;
	myLCD->screen_y = y;
	for(i = 0; i < myLCD->SCREEN_WIDTH / 64; i++) 	{
		writeCommand(myLCD->DISPLAY_SET_Y | 0, i, myLCD);
		writeCommand(myLCD->DISPLAY_SET_X | y, i, myLCD);
		writeCommand(myLCD->DISPLAY_START_LINE | 0, i, myLCD);
	}
	writeCommand(myLCD->DISPLAY_SET_Y | (x % 64), (x / 64), myLCD);
	writeCommand(myLCD->DISPLAY_SET_X | y, (x / 64), myLCD);
}

//-------------------------------------------------------------------------------------------------
//	Write 1 byte of date out to the data pins D0-D7
//-------------------------------------------------------------------------------------------------
void putData(uint8_t data, Ks0108pi* myLCD) {
	bcm2835_gpio_write(myLCD->PIN_D0, (data >> 0) & 1 ) ;
	bcm2835_gpio_write(myLCD->PIN_D1, (data >> 1) & 1 ) ;
	bcm2835_gpio_write(myLCD->PIN_D2, (data >> 2) & 1 ) ;
	bcm2835_gpio_write(myLCD->PIN_D3, (data >> 3) & 1 ) ;
	bcm2835_gpio_write(myLCD->PIN_D4, (data >> 4) & 1 ) ;
	bcm2835_gpio_write(myLCD->PIN_D5, (data >> 5) & 1 ) ;
	bcm2835_gpio_write(myLCD->PIN_D6, (data >> 6) & 1 ) ;
	bcm2835_gpio_write(myLCD->PIN_D7, (data >> 7) & 1 ) ;
}

//-------------------------------------------------------------------------------------------------
// Write data to current position. Low level write to single pixel
//-------------------------------------------------------------------------------------------------
void writeData(uint8_t dataToWrite, Ks0108pi* myLCD) {
	lcdDelay();
	bcm2835_gpio_write(myLCD->PIN_RS, HIGH);
	putData(dataToWrite, myLCD);
	enableController(myLCD->screen_x / 64);
	bcm2835_gpio_write(myLCD->PIN_EN, HIGH);
	lcdDelay();
	bcm2835_gpio_write(myLCD->PIN_EN, LOW);
	disableController(myLCD->screen_x / 64);
	myLCD->screen_x++;
}

//-------------------------------------------------------------------------------------------------
// Write command to specified controller
//-------------------------------------------------------------------------------------------------
void writeCommand(uint8_t commandToWrite, uint8_t controller, Ks0108pi* myLCD) {
	lcdDelay();
	bcm2835_gpio_write(myLCD->PIN_RS, LOW);
	enableController(controller);
	putData(commandToWrite, myLCD);
	bcm2835_gpio_write(myLCD->PIN_EN, HIGH);
	lcdDelay();
	bcm2835_gpio_write(myLCD->PIN_EN, LOW);
	disableController(controller);
}

//-------------------------------------------------------------------------------------------------
// lcdDelay function
//-------------------------------------------------------------------------------------------------
void lcdDelay(void) {
	bcm2835_delayMicroseconds(3);
}

//-------------------------------------------------------------------------------------------------
// Enable/Disable Controller (0-1) - screen controlled by 2 64x64 pixel drivers
//-------------------------------------------------------------------------------------------------
void enableController(uint8_t controller, Ks0108pi* myLCD) {
	switch(controller) {
		case 0 : bcm2835_gpio_write(myLCD->PIN_CS1, HIGH); break;
		case 1 : bcm2835_gpio_write(myLCD->PIN_CS2, HIGH); break;
		default: break;
	}
}

void disableController(uint8_t controller, myLCD) {
	switch(controller){
		case 0 : bcm2835_gpio_write(myLCD->PIN_CS1, LOW); break;
		case 1 : bcm2835_gpio_write(myLCD->PIN_CS2, LOW); break;
		default: break;
	}
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void clearScreen(Ks0108pi* myLCD) {
	clearBuffer(myLCD);
	syncBuffer(myLCD);
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void clearBuffer(Ks0108pi* myLCD) {
	memset(myLCD->framebuffer, 0, myLCD->framebuffer_size);
	//std::fill_n(framebuffer,framebuffer_size, 0);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void syncBuffer(Ks0108pi* myLCD) {
	uint8_t i, j;
	int counter = 0;
	for(j = 0; j < 8; j++)
	{
		goTo(0, j);
		for(i = 0; i < myLCD->SCREEN_WIDTH; i++)
		writeData(/*(uint8_t)*/myLCD->framebuffer[counter++], myLCD);
	}
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void wait(unsigned int millis) {
	bcm2835_delay(millis);
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void setPixel(uint8_t x, uint8_t y, Ks0108pi* myLCD) {
	int idx = (myLCD->SCREEN_WIDTH * (y / 8)) + x;
	myLCD->framebuffer[idx] |= 1 << y % 8;
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void clearPixel(uint8_t x, uint8_t y, Ks0108pi* myLCD) {
	int idx = (myLCD->SCREEN_WIDTH * (y / 8)) + x;
	myLCD->framebuffer[idx] &= ~(1 << y % 8);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void setPixels(uint8_t x, uint8_t y, uint8_t byte, , Ks0108pi* myLCD) {
	int idx = (myLCD->SCREEN_WIDTH * (y / 8)) + x;
	int idx2 = (myLCD->SCREEN_WIDTH * ( (y / 8) + 1) ) + x;
	uint8_t rest = y % 8;
	myLCD->framebuffer[idx] |= ( byte << y % 8 );
	if(rest)
		myLCD->framebuffer[idx2] |= byte >> (8 - y % 8);
}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t style, Ks0108pi* myLCD) {
	for(int nx = x; nx < (x + w); nx++){
		for(int ny = y; ny < (y + h) ; ny++){
			if(style & STYLE_BLACK_BG) setPixel(nx, ny, myLCD);
			else if(style & STYLE_WHITE_BG) clearPixel(nx, ny, myLCD);
			else if(style & STYLE_GRAY_BG){
				if((nx + ny) % 2 == 0)
					clearPixel(nx, ny, myLCD);
				else
					setPixel(nx, ny, myLCD);
			}
		}
	}

	if( (style & STYLE_BLACK_BORDER) || (style & STYLE_WHITE_BORDER)) {
		drawLine(x, y, (x + w) - 1, y, myLCD);
		drawLine(x, (y + h) - 1, (x + w) - 1, (y + h) - 1, myLCD);
		drawLine(x, y, x, (y + h) - 1, myLCD);
		drawLine((x + w) - 1, y, (x + w) - 1, (y + h) - 1, myLCD);
	}
}


//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, Ks0108pi* myLCD) {
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, 
		  e2;

	while(1) {
		setPixel(x0, y0, myLCD);
		if (x0 == x1 && y0 == y1) break;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}

}

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void writeChar(uint8_t x, uint8_t y, char charToWrite, uint8_t* font, Ks0108pi* myLCD) {
	int firstChar = font[4];
	int charCount = font[5];
	int charHeight = font[3];
	int charWidth = font[2];
	int sum = 6;
	int fixed_width = 1;

	if( (font[0] + font [1]) != 0x00){
		fixed_width  = 0;
	}


	if( !fixed_width ){
		charWidth = font[6 + (charToWrite - firstChar)];
		sum += charCount;
	}

	//jumps to the char data position on the array.
	for(int i = firstChar; i<charToWrite; i++){
		if( !fixed_width )
			sum += font[6 + i - firstChar] * ceil(charHeight / 8.0);
		else
			sum += charWidth * ceil(charHeight / 8.0);
	}

	for(int line = 0; line < charHeight; line += 8){
		for(int col = 0; col<charWidth; col++){
			setPixels(x + col, ceil(y + line), font[sum + col + (int)ceil(charWidth * line / 8.0)], myLCD );
		}
	}

}



//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------
void writeString(uint8_t x, uint8_t y, char * stringToWrite, uint8_t* font, , Ks0108pi* myLCD) {
	while(*stringToWrite){
		writeChar(x,y,*stringToWrite++, font, myLCD);
		x+=font[2]+1;
	}
}

void shiftBufferHorizontal(int x, Ks0108pi* myLCD) {  
	//uint8_t *originalfb = new uint8_t[framebuffer_size];	// NEW? not C?
  uint8_t *originalfb = malloc(myLCD->framebuffer_size * sizeof(uint8_t));   // free(originalfb)

	//backup of current framebuffer
	memcpy(originalfb, myLCD->framebuffer, framebuffer_size * sizeof(uint8_t));
  //std::copy(myLCD.framebuffer, myLCD.framebuffer + myLCD.framebuffer_size, originalfb); // *** find C equivalent
	clearBuffer(); // clear main framebuffer

	int x_original;
	int x_new;


	// line scan
	for(int y = 0; y < SCREEN_HEIGHT / 8; y++) 	{
		//x scan
		x_original = x < 0 ? x * -1 : 0;
		x_new = x < 0 ? 0 : x ;
		while(x_original < SCREEN_WIDTH && x_new < SCREEN_WIDTH) {
			setPixels(x_new, y * 8, originalfb[ (y * myLCD->SCREEN_WIDTH) + x_original ], myLCD );	// *** Update to remove THIS
			x_original++;
			x_new++;
		}
	}
  free(originalfb);
}
