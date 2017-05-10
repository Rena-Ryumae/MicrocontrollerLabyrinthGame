#include "maze_gen.h"
#include <fsl_debug_console.h>
#include <stdlib.h>
#include <time.h>

int n;

int random(int n) {
	int random_number = rand() % n;
	return random_number;
}

int get_next(int ** b, int x, int y, int num, int n, int checked) {
	debug_printf("Our y is %d \r\n", y);
	debug_printf("In Next checked = %d \r\n", checked);
	debug_printf("NUM = %d \r\n", num);
	if (checked > 4) {
		debug_printf("Returned Null for next \r\n");
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
		debug_printf("Moving right \r\n");
		return 1;
	}
	else if (num == 1) {
		y--;
		debug_printf("In first if \r\n");
		debug_printf("next y is %d \r\n", y);
		if (y < 0) {
			debug_printf("In nested if \r\n");
			num++;
			num = num % 4;
			int next = get_next(b, x, y+1, num, n, checked+1);
			debug_printf("next is %d \r\n", next);
			return next;
		}
		else if (b[x][y] > 15) {
			num++;
			num = num % 4;
			int next = get_next(b, x, y+1, num, n, checked+1);
			debug_printf("next is %d \r\n", next);
			return next;
		}
		debug_printf("Moving up \r\n");
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
		debug_printf("Moving left \r\n");
		return 2;
	}
	else {
		y++;
		if ((y >= n) || (b[x][y] > 7)) {
			num++;
			num = num % 4;
			return get_next(b, x, y-1, num, n, checked+1);
		}
		debug_printf("Moving down \r\n");
		return 3;
	}
}

int ** gen_maze(int n) {
	int i;
	int **board;
	board = malloc(n*sizeof(int *));
	if (board == NULL) {
		int **p;
		int *c;
		**p = *c;
		*c = -1;
		return p;
	}
	for (i = 0; i < n; i++) {
		board[i] = malloc(n*sizeof(int));
		if (board[i] == NULL) {
			int **p;
			int *c;
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
			int **p;
			int *c;
			**p = *c;
			*c = -1;
			return p;
		}
	int x = 0;
	int y = 0;
	board[x][y] |= 0x10;
	queue[list_tracker] = x;
	queue[list_tracker++] = y;
	list_tracker++;
	int num = random(4);
	int next = get_next(board, x, y, num, n, 1);
	if (next == 1) { 
		board[x][y] &= 0x1D;
		x++;
		board[x][y] &= 0x1C;
		board[x][y] |= 0x10;
		}
	else if (next == 2) { 
		board[x][y] &= 0x1C;
		x--;
		board[x][y] &= 0x1D;
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
	queue[list_tracker++] = y;
	list_tracker++;
	debug_printf("Beginning of while \r\n");
	int f = 1;
	while (list_tracker != 0) {
		debug_printf("While loop iteration %d \r\n", f);
		f++;
		int num = random(4);
		debug_printf("Y in while loop is %d \r\n", y);
		int next = get_next(board, x, y, num, n, 1);
		debug_printf("Got next = %d \r\n", next);
		if (next == NULL) {
			debug_printf("In Null if");
			y = queue[list_tracker--];
			x = queue[list_tracker--];
		}
		else {
			debug_printf("in else");
			if (next == 1) { 
			board[x][y] &= 0x1D;
			x++;
			board[x][y] &= 0x1C;
			board[x][y] |= 0x10;
			}
			else if (next == 2) { 
			board[x][y] &= 0x1C;
			x--;
			board[x][y] &= 0x1D;
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
			debug_printf("Finished while loop iteration \r\n \r\n");
			
			queue[list_tracker] = x;
			queue[list_tracker++] = y;
			list_tracker++;
		}
	}
	debug_printf("out of while");
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			board[x][y] &= 0x0F;
		}
	}
	debug_printf("Finished maze_gen");
	return board;
}