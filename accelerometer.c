#include <Board_Accelerometer.h>
#include <Board_Magnetometer.h>
#include <fsl_debug_console.h>
#include <board.h>
#include "utils.h"

ACCELEROMETER_STATE state1;
ACCELEROMETER_STATE state2;
ACCELEROMETER_STATE state3;


MAGNETOMETER_STATE mstate;
void mediumDelay() {delay(); delay(); delay(); delay();}

int main () {
	hardware_init();
	Accelerometer_Initialize();
	Magnetometer_Initialize();
	LED_Initialize();
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
		if (y < -1200) {LED_Off();LEDBlue_On();} //UP
		else if (y > 1200) {LED_Off();LEDGreen_On();} //DOWN
		else if (x < -1200) {LED_Off();LEDRed_On();} //LEFT
		else if (x > 1200) {LED_Off();LEDRed_On();} //RIGHT
		else {
			LED_Off(); //"Flat"
		}
		
		debug_printf("State 1: %5d %5d %5d\r\n", state1.x, state1.y, state1.z);
		debug_printf("State 2: %5d %5d %5d\r\n", state2.x, state2.y, state2.z);
		debug_printf("State 3: %5d %5d %5d\r\n", state3.x, state3.y, state3.z);
		mediumDelay();
	}
	
	
}