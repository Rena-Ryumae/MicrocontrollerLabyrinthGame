#include <stdlib.h>


typedef struct ball_state {
	// Ball's current location
	int x;
	int y;
} ball;

typedef struct box_state {
	// Box's location
	int x;
	int y;
	int filled; // 1 if ball is in box, 0 otherwise
	unsigned int wall; // 4 bits, (up, down, left, right)
} box;

typedef struct board_state {
	// Ball's starting location
	int startx;
	int starty;
	// Ball's goal location
	int finishx;
	int finishy;
	struct box_state maze[4][4]; // Array of boxes 2x2 array
	int finish; // 1 if ball reached goal location
} board;


void initializeBall(ball * b) {
	b->x = 0;
	b->y = 0;
	return;
}

box initializeBox(unsigned int w, ball * b, box * bx, int ycoord, int xcoord) {
	bx->wall = w;
	bx->x = xcoord;
	bx->y = ycoord;
	if ((bx->x == 0) && (bx->y == 0)) {
		bx->filled = 1;
	} else {
		bx->filled = 0;
	}
	return *bx;
}

void initializeBoard(int finx, int finy, unsigned int walls[4][4], board * brd, ball * b) {
	brd->startx = 0;
	brd->starty = 0;
	brd->finishx = finx;
	brd->finishy = finy;
	brd->finish = 0;
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			box * bx = malloc(sizeof (box));
			int wall = walls[row][col];
			brd->maze[row][col] = initializeBox(wall, b, bx, row, col);	
		}
	}
}

void update(int newy, int newx, board * brd, ball * b) {
	b->x = newx;
	b->y = newy;
	brd->maze[b->y][b->x].filled = 1;
}

void moveBall(unsigned int d, ball * b, board * brd) {
	unsigned int check = brd->maze[b->y][b->x].wall & d;
	// No wall, ball can move there
	if (check != d) {
		brd->maze[b->y][b->x].filled = 0;
		if (d == 1) update(b->y, b->x + 1, brd, b);			// Move right
		else if (d == 2) update(b->y, b->x - 1, brd, b); // Move left
		else if (d == 4) update(b->y + 1, b->x, brd, b); // Move down
		else update(b->y - 1, b->x, brd, b); // Move up
		if ((brd->finishx == b->x) && (brd->finishy == b->y)) {brd->finish = 1;}
	}
}