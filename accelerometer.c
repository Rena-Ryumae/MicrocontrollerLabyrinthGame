#include <Board_Accelerometer.h>
#include <Board_Magnetometer.h>
#include <fsl_debug_console.h>
#include <board.h>
#include "utils.h"
#include "game_structs.h"

ACCELEROMETER_STATE state1;
ACCELEROMETER_STATE state2;
ACCELEROMETER_STATE state3;

void tmpprint (board * brd) {
	int w;
	debug_printf("------------------------\r\n");
  for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			w = brd->maze[row][col].wall;
			if (w == 0)      {debug_printf("   %d   ", brd->maze[row][col].filled);}
			else if (w == 1) {debug_printf("   %d  |", brd->maze[row][col].filled);}
			else if (w == 2) {debug_printf("|  %d   ", brd->maze[row][col].filled);}
			else if (w == 3) {debug_printf("|  %d  |", brd->maze[row][col].filled);}
			else if (w == 4) {debug_printf("___%d__", brd->maze[row][col].filled);}
			else if (w == 5) {debug_printf("___%d__|", brd->maze[row][col].filled);}
			else if (w == 6) {debug_printf("|__%d__", brd->maze[row][col].filled);}
			else if (w == 7) {debug_printf("|__%d__|", brd->maze[row][col].filled);}
			else if (w == 8) {debug_printf("   %d   ", brd->maze[row][col].filled);}
			else if (w == 9) {debug_printf("   %d  |", brd->maze[row][col].filled);}
			else if (w == 10){debug_printf("|  %d   ", brd->maze[row][col].filled);}
			else if (w == 11){debug_printf("|  %d  |", brd->maze[row][col].filled);}
			else if (w == 12){debug_printf("___%d__", brd->maze[row][col].filled);}
			else if (w == 13){debug_printf("___%d__|", brd->maze[row][col].filled);}
			else if (w == 14){debug_printf("|__%d__", brd->maze[row][col].filled);}
			else if (w == 15){debug_printf("|__%d__|", brd->maze[row][col].filled);}
		}
		debug_printf("\r\n");
	}
}	

void mediumDelay() {
  for (int i = 0; i < 6; i++) {
		delay(); delay(); delay(); delay(); delay();
	}
}

int main () {
	hardware_init();
	Accelerometer_Initialize();
	LED_Initialize();
	int fx = 1;
	int fy = 1;
	unsigned int walls[4][4] = {{11,10,12, 9},{3, 7,11, 3},{6, 12,1, 3},{14,13,6,5}};
	ball * b = malloc(sizeof (ball));
	board * brd = malloc(sizeof (board));
	initializeBall(b);
	initializeBoard(fx, fy, walls, brd, b);
	
	
	int x;
	int y;
	while(1) {
		Accelerometer_GetState(&state1);
		Accelerometer_GetState(&state2);
		Accelerometer_GetState(&state3);
		// States are this way due to orientation of the board
		y = (state1.x + state2.x + state3.x);
		x = (state1.y + state2.y + state3.y);
		if (brd->finish == 1) {LED_Off();return 0;}
		if (y < -1200) {LED_Off();LEDBlue_On();moveBall(8, b, brd);} //UP
		else if (y > 1200) {LED_Off();LEDGreen_On();moveBall(4,b,brd);} //DOWN
		else if (x < -1200) {LED_Off();LEDRed_On();moveBall(2,b,brd);} //LEFT
		else if (x > 1200) {LED_Off();LEDRed_On();moveBall(1,b,brd);} //RIGHT
		else {
			LED_Off(); //"Flat"
		}
		tmpprint(brd);
		/*debug_printf("------------------------\r\n");
		
		for (int row = 0; row < 4; row++) {
				debug_printf("%d %d %d %d\r\n", brd->maze[row][0].filled, brd->maze[row][1].filled, brd->maze[row][2].filled, brd->maze[row][3].filled);
		}*/
		
		/*debug_printf("State 1: %5d %5d %5d\r\n", state1.x, state1.y, state1.z);
		debug_printf("State 2: %5d %5d %5d\r\n", state2.x, state2.y, state2.z);
		debug_printf("State 3: %5d %5d %5d\r\n", state3.x, state3.y, state3.z);*/
		mediumDelay();
	}
	
	
}