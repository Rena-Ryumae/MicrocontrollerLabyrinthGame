#ifndef __GAME_STRUCTS__
#define __GAME_STRUCTS__

extern int finx;
extern int finy;
extern int **walls;
extern int newx;
extern int newy;
extern unsigned int d;

typedef struct ball_state {
	// Ball's current location
	int x;
	int y;
} ball;

typedef struct board_state {
	// Ball's starting location
	int startx;
	int starty;
	// Ball's goal location
	int finishx;
	int finishy;
	//struct box_state maze[4][4]; // Array of boxes 2x2 array
	unsigned int maze[3][3];
	int finish; // 1 if ball reached goal location
} board;

void initializeBall(ball * b);

void initializeBoard(int finx, int finy, int **walls, board * brd, ball * b);

void update(int newy, int newx, board * brd, ball * b);

void moveBall(unsigned int d, ball * b, board * brd);

#endif