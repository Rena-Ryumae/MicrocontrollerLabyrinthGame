#include "maze_gen.h"
#include <fsl_debug_console.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "game_structs.h"

int n;
time_t seed;
int random(int n) {
	
	int random_number = rand() % n;
	return random_number;
}

int get_next(char ** b, int x, int y, int num, int n, int checked) {
	if (checked > 4) {
		return NULL;
	}
	if (num == 0) {
		x++;
		if (x >= n) {
			num++;
			num = num % 4;
			int next = get_next(b, x-1, y, num, n, checked+1);
			return next;
		}
		else if (b[x][y] > 15) {
			num++;
			num = num % 4;
			int next = get_next(b, x-1, y, num, n, checked+1);
			return next;
		}
		return 1;
	}
	else if (num == 1) {
		y--;
		if (y < 0) {
			num++;
			num = num % 4;
			int next = get_next(b, x, y+1, num, n, checked+1);
			return next;
		}
		else if (b[x][y] > 15) {
			num++;
			num = num % 4;
			int next = get_next(b, x, y+1, num, n, checked+1);
			return next;
		}
		return 4;
	}
	else if (num == 2) {
		x--;
		if (x < 0) {
			num++;
			num = num % 4;
			int next = get_next(b, x+1, y, num, n, checked+1);
			return next;
		}
		else if (b[x][y] > 15) {
			num++;
			num = num % 4;
			int next = get_next(b, x+1, y, num, n, checked+1);
			return next;
		}
		return 2;
	}
	else {
		y++;
		if (y >= n) {
			num++;
			num = num % 4;
			int next = get_next(b, x, y-1, num, n, checked+1);
			return next;
		}
		else if (b[x][y] > 15) {
			num++;
			num = num % 4;
			int next = get_next(b, x, y-1, num, n, checked+1);
			return next;
		}
		return 3;
	}
}

char ** gen_maze(int n, final * fin, int sdtime) {
	srand(sdtime);
	int i;
	char **board;
	board = malloc(n*sizeof(char *));
	if (board == NULL) {
		char **p;
		char *c;
		**p = *c;
		*c = -1;
		return p;
	}
	for (i = 0; i < n; i++) {
		board[i] = malloc(n*sizeof(char));
		if (board[i] == NULL) {
			char **p;
			char *c;
			**p = *c;
			*c = -1;
			return p; }
	}
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			board[x][y] = 15;
		}
	}
	int list_tracker = 0;
	int *queue = malloc(2*n*n*sizeof(int));
		if (queue == NULL) {
			char **p;
			char *c;
			**p = *c;
			*c = -1;
			return p;
		}
	int x = 0;
	int y = 0;
	board[x][y] |= 0x10;
	queue[list_tracker] = x;
	list_tracker = list_tracker + 1;
	queue[list_tracker] = y;
	list_tracker++;
	int num = random(4);
	int next = get_next(board, x, y, num, n, 1);
	if (next == 1) { 
		board[x][y] &= 0x1E;
		x++;
		board[x][y] &= 0x1D;
		board[x][y] |= 0x10;
		}
	else if (next == 2) { 
		board[x][y] &= 0x1D;
		x--;
		board[x][y] &= 0x1E;
		board[x][y] |= 0x10;
		}
	else if (next == 3) { 
		board[x][y] &= 0x1B;
		y++;
		board[x][y] &= 0x17;
		board[x][y] |= 0x10;
		}
	else { 
		board[x][y] &= 0x17;
		y--;
		board[x][y] &= 0x1B;
		board[x][y] |= 0x10;
		}
	queue[list_tracker] = x;
	list_tracker = list_tracker + 1;
	queue[list_tracker] = y;
	list_tracker++;
	while (list_tracker != 0) {
		int num = random(4);
		int next = get_next(board, x, y, num, n, 1);
		if (next == NULL) {
			list_tracker = list_tracker - 1;
			y = queue[list_tracker];
			list_tracker = list_tracker - 1;
			x = queue[list_tracker];
			if (fin->found == 0) {
				fin->fx = x;
				fin->fy = y;
				fin->found = 1;
			}
		}
		else {
			if (next == 1) { 
			board[x][y] &= 0x1E;
			x++;
			board[x][y] &= 0x1D;
			board[x][y] |= 0x10;
			}
			else if (next == 2) { 
			board[x][y] &= 0x1D;
			x--;
			board[x][y] &= 0x1E;
			board[x][y] |= 0x10;
			}
			else if (next == 3) { 
			board[x][y] &= 0x1B;
			y++;
			board[x][y] &= 0x17;
			board[x][y] |= 0x10;
			}
			else { 
			board[x][y] &= 0x17;
			y--;
			board[x][y] &= 0x1B;
			board[x][y] |= 0x10;
			}
			
			queue[list_tracker] = x;
			list_tracker = list_tracker + 1;
			queue[list_tracker] = y;
			list_tracker++;
		}
	}
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			board[x][y] &= 0x0F;
		}
	}
	return board;
}