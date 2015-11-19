#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include "KS0108.h"  

#include "fonts/metric01.h"  
#include "fonts/metric02.h"  
#include "fonts/metric04.h"  
#include "fonts/pbar.h"  
#include "fonts/pbar2.h"

#include "bmps/chuva.h"  

#include <math.h>
#define PI 3.14159265

void drawPbar(uint8_t x, uint8_t y, uint8_t percent){
	
	char string[30];
	int tiles = ((120.0*percent)/100.0)/4;
	for(int i=0; i<30; i++){
		if 		(i==0)string[0] = tiles < 1 ? PBAR_INI : PBAR_INI_FILL;
		else if (i==29)string[29] = tiles < 30 ? PBAR_END : PBAR_END_FILL;
		else if (i==tiles) string[i] = PBAR_FILL_LAST;
		else string[i] = i <= tiles ? PBAR_FILL : PBAR_EMPTY;
	}
	for(int i=0; i<30; i++){
		GLCD_WriteChar(x,y,string[i], pbar);
		x+=pbar[2];
	}
}

int main(int argc, char** argv){

	GLCD_Initalize();
	GLCD_ClearScreen();

	char txt[20];
	for(int i=0; i<=100 ; i++){
		GLCD_ClearBuffer();
		drawPbar(4,6,i);
		sprintf(txt,"DOWNLOAD %d%%",i);
		GLCD_WriteString(6,0,txt,metric01);
		GLCD_WriteString(50,35,"SCANING...",metric01);
		GLCD_SyncBuffer();
		bcm2835_delay(50);
	}
	

}

/*char string[10];
	GLCD_ClearScreen();
	for(int i=2000; i<3000; i++){
		sprintf(string,"%d<",i);
		GLCD_ClearBuffer();
			GLCD_WriteString(0,0,"ENERGIA" , metric02);
			GLCD_WriteString(64,0,"USO", metric01);
			GLCD_WriteString(64,7,"HOJE", metric01);
			GLCD_WriteString(0,18,string, metric04);
			GLCD_WriteString(0,52,"20.1*" , metric02);
			GLCD_WriteString(42,52,"MIN 12*", metric01);
			GLCD_WriteString(42,59,"MAX 28*", metric01);
			GLCD_DrawLine(0,48,127,48);
		GLCD_SyncBuffer();
		bcm2835_delay(100);
	}

*/