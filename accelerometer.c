#include <Board_Accelerometer.h>
#include <fsl_debug_console.h>
#include <board.h>
#include "utils.h"
#include "game_structs.h"
#include "maze_gen.h"
#include "challenge1.h"

ACCELEROMETER_STATE state1;
ACCELEROMETER_STATE state2;
ACCELEROMETER_STATE state3;

int volatile sec = 0;

// Medium Delay between game rounds
void mediumDelay() {
  for (int i = 0; i < 3; i++) {
		delay(); delay(); delay(); delay(); delay();
	}
}

// Initialize timer for seed values for random
void Timer_Initialize() {
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; //Enable clock to PIT module
	PIT->MCR = 1; //Enables clock for PIT timer
	PIT->CHANNEL[0].TCTRL |= (1 << 1); //Enables interrupts
	PIT->CHANNEL[0].TCTRL |= (0 << 0); //Disable timer first
	PIT->CHANNEL[0].LDVAL = 70970000; //Set load value of 0th PIT
	PIT->CHANNEL[0].TCTRL |= (1 << 0); //Start timer
	NVIC_EnableIRQ(PIT0_IRQn);
}

// ISR for timer
void PIT0_IRQHandler() {
	sec=sec+1;
	PIT->CHANNEL[0].TFLG = 1;
}

// Determines which game mode should be played
// Returns 0 if the normal game mode is chosen
// Returns 1 if the challenge game mode is chosen
int game_mode() {
	int button;
	int notready = 1;
	int x;
	int y;
	while(notready) {
		Accelerometer_GetState(&state1);
		Accelerometer_GetState(&state2);
		Accelerometer_GetState(&state3);
		// States are this way due to orientation of the board
		y = (state1.x + state2.x + state3.x);
		x = (state1.y + state2.y + state3.y);
		if (y < -1200) {
			LED_Off();LEDBlue_On();
			button = 0;
			notready = 0;
		} //UP indicates normal game mode
		else if (y > 1200) {
			LED_Off();LEDGreen_On();
			button = 1;
			notready = 0;
		} //DOWN indicates challenge game mode
	}
	return button;
}

// Each maze
int gamePlay(int button, int g) {
	int n;
	int finx;
	int finy;
	ball * b = malloc(sizeof (ball));
	initializeBall(b);
	board * brd;
	// Normal game play mode
	if (button == 0) {
		n = 7;
		final * f = malloc(sizeof (final));
		f->found = 0;
		int sdtime = PIT->CHANNEL[0].CVAL;
		// Uses random maze generation algorithm to generate maze
		char ** walls = gen_maze(n, f, sdtime);
		finx = f->fx;
		finy = f->fy;
		free(f);
		brd = malloc(sizeof (board));
		initializeBoard(finx, finy, walls, brd, b, n);
		for (int i = 0; i < n; i++) {
			free(walls[i]);
		}
		free(walls);
	} 
	// Challenge game play mode
	else {
		n = 15;
		brd = malloc(sizeof (board));
		char ** challenge1;
		if (g == 0) {
			challenge1 = createChallenge();
			finx = 7;
			finy = 14;
		}
		else {
			challenge1 = createChallenge2();
			finx = 7;
			finy = 0;
		}
		initializeBoard(finx, finy, challenge1, brd, b, n);
		for (int i = 0; i < n; i++) {
			free(challenge1[i]);
		}
		free(challenge1);
	}
	int s = 1;
	// Main loop
	while(1) {
		Accelerometer_GetState(&state1);
		Accelerometer_GetState(&state2);
		Accelerometer_GetState(&state3);
		int x;
		int y;
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
		// Send initial information
		if (s == 1) {
			if (n < 10) {debug_printf("0%d\r\n",n);}
			else {debug_printf("%d\r\n",n);}
			if (finx < 10) {debug_printf("0%d\r\n",finx);}
			else {debug_printf("%d\r\n",finx);}
			if (finy < 10) {debug_printf("0%d\r\n",finy);}
			else {debug_printf("%d\r\n",finy);}
			s = 0;
		}
		// Send board maze information
		for (int col = 0; col < n; col++) {
			for (int row = 0; row < n; row++) {
				debug_printf("%d ", brd->maze[row][col]);
			}
			debug_printf("\r\n");
		}
		// Send finish signal when complete
		if (brd->finish == 1) {
			debug_printf("99999\r\n");
			free(b);
			free(brd);
			return (1 - g);
		}
		mediumDelay();
	}
	
}

// Main initiates full game
int main () {
	// Initialization
	hardware_init();
	Accelerometer_Initialize();
	LED_Initialize();
	Timer_Initialize();
	// Sends signal that game is reset
	debug_printf("9999999\r\n");
	// Determines game mode
	int button = game_mode();
	int g = 0;
	// Each maze game
	while(1) {
		g = gamePlay(button, g);
		mediumDelay();
		mediumDelay();
	}
}
