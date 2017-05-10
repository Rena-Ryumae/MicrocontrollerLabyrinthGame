#include <stdlib.h>
#include "game_structs.h"
#include <fsl_debug_console.h>

void initializeBall(ball * b) {
	b->x = 0;
	b->y = 0;
	return;
}

void initializeBoard(int finx, int finy, int **walls, board * brd, ball * b) {
	brd->startx = 0;
	brd->starty = 0;
	brd->finishx = finx;
	brd->finishy = finy;
	brd->finish = 0;
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			//box * bx = malloc(sizeof (box));
			//brd->maze[row][col] = initializeBox(wall, b, bx, row, col);	
			brd->maze[row][col] &= 0x00;
			brd->maze[row][col] |= walls[row][col]; 
			if (row == 0 && col == 0) {
				brd->maze[row][col] &= 0x0F; // Give filled 0 
				brd->maze[row][col] |= 0x10; // Give filled 1
			}
			else {
				brd->maze[row][col] &= 0x0F; // Give filled 0 
			}
		}
	}
}

void update(int newx, int newy, board * brd, ball * b) {
	b->x = newx;
	b->y = newy;
	brd->maze[b->x][b->y] |= 0x10; // Filled value becomes 1
}

void moveBall(unsigned int d, ball * b, board * brd) {
	unsigned int wall = brd->maze[b->x][b->y] & 0x0F; //Gets last 4 bits (wall)
	unsigned int check = wall & d;
	// No wall, ball can move there
	if (check != d) {
		brd->maze[b->x][b->y] &= 0x0F; // Change filled to 0
		if (d == 1) update(b->x + 1, b->y, brd, b);			// Move right
		else if (d == 2) update(b->x - 1, b->y, brd, b); // Move left
		else if (d == 4) update(b->x, b->y + 1, brd, b); // Move down
		else update(b->x, b->y - 1, brd, b); // Move up
		if ((brd->finishx == b->x) && (brd->finishy == b->y)) {brd->finish = 1;}
	}
}