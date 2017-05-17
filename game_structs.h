#ifndef __GAME_STRUCTS__
#define __GAME_STRUCTS__

extern int n;
extern int finx;
extern int finy;
extern char **walls;
extern int newx;
extern int newy;
extern unsigned char d;

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
	unsigned char maze[15][15];
	int finish; // 1 if ball reached goal location
} board;

void initializeBall(ball * b);

void initializeBoard(int finx, int finy, char **walls, board * brd, ball * b, int n);

void update(int newy, int newx, board * brd, ball * b);

void moveBall(unsigned char d, ball * b, board * brd);

#endif