#include <Board_Accelerometer.h>
#include <fsl_debug_console.h>
#include <board.h>
#include "utils.h"
#include "game_structs.h"
#include "maze_gen.h"

ACCELEROMETER_STATE state1;
ACCELEROMETER_STATE state2;
ACCELEROMETER_STATE state3;

void tmpprint (board * brd) {
	unsigned int w;
	unsigned int f;
	debug_printf("------------------------\r\n");
  for (int row = 0; row < 10; row++) {
		for (int col = 0; col < 10; col++) {
			w = brd->maze[row][col] & 0x0F;
			f = brd->maze[row][col] >> 4;
			if (w == 0)      {debug_printf("   %d   ", f);}
			else if (w == 1) {debug_printf("   %d  |", f);}
			else if (w == 2) {debug_printf("|  %d   ", f);}
			else if (w == 3) {debug_printf("|  %d  |", f);}
			else if (w == 4) {debug_printf("___%d__", f);}
			else if (w == 5) {debug_printf("___%d__|", f);}
			else if (w == 6) {debug_printf("|__%d__", f);}
			else if (w == 7) {debug_printf("|__%d__|", f);}
			else if (w == 8) {debug_printf("   %d   ", f);}
			else if (w == 9) {debug_printf("   %d  |", f);}
			else if (w == 10){debug_printf("|  %d   ", f);}
			else if (w == 11){debug_printf("|  %d  |", f);}
			else if (w == 12){debug_printf("___%d__", f);}
			else if (w == 13){debug_printf("___%d__|", f);}
			else if (w == 14){debug_printf("|__%d__", f);}
			else if (w == 15){debug_printf("|__%d__|", f);}
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
	int n = 3;
	final * f = malloc(sizeof (final));
	f->found = 0;
	int ** walls = gen_maze(n, f);
	int s = 1;
	/*unsigned int walls[10][10] = 
    {
      {11,14,8,12,12,8,13,10,9,15},
		  {3,14,4,8,13,3,14,1,2,13},
		  {6,12,9,2,9,2,9,3,3,15},
			{11,11,3,6,1,2,5,3,6,9},
			{2,5,3,14,1,3,14,1,10,5},
			{2,12,5,10,5,6,9,3,2,9},
			{6,9,10,4,9,10,1,3,7,3},
			{14,1,6,13,3,2,5,3,10,5},
			{15,3,10,12,1,6,12,5,6,9},
		  {6,4,4,13,6,12,12,12,13,7}
		};*/
	ball * b = malloc(sizeof (ball));
	board * brd = malloc(sizeof (board));
	initializeBall(b);
	initializeBoard(f->fx, f->fy, walls, brd, b);
	
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
		
		if (s == 1) {
			debug_printf("%d\r\n",f->fx);
			debug_printf("%d\r\n",f->fy);
			s = 0;
		}
		for (int col = 0; col < 3; col++) {
			for (int row = 0; row < 3; row++) {
				debug_printf("%d ", brd->maze[row][col]);
			}
			debug_printf("\r\n");
		}
		if (brd->finish == 1) {
			debug_printf("99999\r\n");
		}
		mediumDelay();
	}
	
	
}