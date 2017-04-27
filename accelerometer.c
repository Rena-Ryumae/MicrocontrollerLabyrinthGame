#include <Board_Accelerometer.h>
#include <Board_Magnetometer.h>
#include <fsl_debug_console.h>
#include <board.h>
#include "utils.h"
#include "game_structs.h"

ACCELEROMETER_STATE state1;
ACCELEROMETER_STATE state2;
ACCELEROMETER_STATE state3;

//void tmpprint 

void mediumDelay() {delay(); delay(); delay(); delay();}

int main () {
	hardware_init();
	Accelerometer_Initialize();
	LED_Initialize();
	int fx = 1;
	int fy = 1;
	unsigned int walls[2][2] = {{10,9},{6,5}};
	ball * b = malloc(sizeof (ball));
	board * brd = malloc(sizeof (board));
	initializeBall(b);
	initializeBoard(fx, fy, walls, brd, b);
	
	
	Accelerometer_GetState(&state1);
	Accelerometer_GetState(&state2);
	Accelerometer_GetState(&state3);
	int x;
	int y;
	while(1) {
		state1 = state2;
		state2 = state3;
		Accelerometer_GetState(&state3);
		// States are this way due to orientation of the board
		y = (state1.x + state2.x + state3.x);
		x = (state1.y + state2.y + state3.y);
		if (y < -1200) {LED_Off();LEDBlue_On();moveBall(8, b, brd);} //UP
		else if (y > 1200) {LED_Off();LEDGreen_On();moveBall(4,b,brd);} //DOWN
		else if (x < -1200) {LED_Off();LEDRed_On();moveBall(2,b,brd);} //LEFT
		else if (x > 1200) {LED_Off();LEDRed_On();moveBall(1,b,brd);} //RIGHT
		else {
			LED_Off(); //"Flat"
		}
		
		debug_printf("------------------------\r\n");
		
		for (int row = 0; row < 2; row++) {
				debug_printf("%d %d\r\n", brd->maze[row][0].filled, brd->maze[row][1].filled);
		}
		
		/*debug_printf("State 1: %5d %5d %5d\r\n", state1.x, state1.y, state1.z);
		debug_printf("State 2: %5d %5d %5d\r\n", state2.x, state2.y, state2.z);
		debug_printf("State 3: %5d %5d %5d\r\n", state3.x, state3.y, state3.z);*/
		mediumDelay();
		mediumDelay();
		mediumDelay();
	}
	
	
}