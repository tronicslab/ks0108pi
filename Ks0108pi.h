#ifndef __KS0108PI_H_INCLUDED__
#define __KS0108PI_H_INCLUDED__

#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct {

	// static ks0108 commands
	const uint8_t DISPLAY_SET_Y;
	const uint8_t DISPLAY_SET_X;
	const uint8_t DISPLAY_START_LINE;
	const uint8_t DISPLAY_ON_CMD;
	const uint8_t ON;
	const uint8_t OFF;

	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;


	// pins
	uint8_t PIN_RS;
	uint8_t PIN_EN;
	uint8_t PIN_CS1;
	uint8_t PIN_CS2;
	uint8_t PIN_D0;
	uint8_t PIN_D1;
	uint8_t PIN_D2;
	uint8_t PIN_D3;
	uint8_t PIN_D4;
	uint8_t PIN_D5;
	uint8_t PIN_D6;
	uint8_t PIN_D7;

	uint8_t screen_x, screen_y;
	uint8_t *framebuffer;
	int framebuffer_size;

} Ks0108pi;

enum DrawStyle {
	STYLE_WHITE_BG 		 = 1 << 0,
	STYLE_BLACK_BG 		 = 1 << 1,
	STYLE_GRAY_BG 		 = 1 << 2,
	STYLE_WHITE_BORDER = 1 << 3,
	STYLE_BLACK_BORDER = 1 << 4,
};

void close(Ks0108pi* myLCD);
void goTo(uint8_t x, uint8_t y, Ks0108pi *myLCD);
void putData(uint8_t data, Ks0108pi *myLCD);
void writeData(uint8_t dataToWrite, Ks0108pi *myLCD);
void writeCommand(uint8_t commandToWrite, uint8_t controller, Ks0108pi *myLCD);
void lcdDelay();
void enableController(uint8_t controller, Ks0108pi *myLCD);
void disableController(uint8_t controller, Ks0108pi *myLCD);

int init(Ks0108pi *myLCD);
void clearScreen(Ks0108pi *myLCD);
void clearBuffer(Ks0108pi *myLCD);
void syncBuffer(Ks0108pi *myLCD);
void wait(unsigned int millis);

void setPixel(uint8_t x, uint8_t y, Ks0108pi *myLCD);
void clearPixel(uint8_t x, uint8_t y, Ks0108pi *myLCD);
void setPixels(uint8_t x, uint8_t y, uint8_t byte, Ks0108pi *myLCD);

void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t style, Ks0108pi *myLCD);
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, Ks0108pi *myLCD);

void writeChar(uint8_t x, uint8_t y, char charToWrite, uint8_t* font, Ks0108pi *myLCD);
void writeString(uint8_t x, uint8_t y, char * stringToWrite, uint8_t* font, Ks0108pi *myLCD);

void shiftBufferHorizontal(int x, Ks0108pi *myLCD);

#endif // __KS0108PI_H_INCLUDED__
