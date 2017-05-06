#include "maze_gen.h"
#include <stdlib.h>
#include <time.h>

int n;
struct maze_node *backtrack_queue = NULL;

int random(int n) {
	srand(time(NULL));
	int random_number = rand() % n;
	return random_number;
}

node ***init_board(int n) {
	int i;
	node ***board;
	board = malloc(n*sizeof(int *));
	for (i = 0; i < n; i++) {
		board[i] = (node **)malloc(n*sizeof(node));
	}
	return board;
}

void init_nodes(node*** p, int n) {
	node *dummy = malloc(sizeof(node));
	dummy->x = -1;
	dummy->y = -1;
	dummy->visited = 1;
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			int f_neighbors = 4;
			int l_wall = 1;
			int r_wall = 1;
			int t_wall = 1;
			int b_wall = 1;
			node *l_neighbor = dummy;
			node *r_neighbor = dummy;
			node *t_neighbor = dummy;
			node *b_neighbor = dummy;
			if (x == 0) {
				f_neighbors--;
				l_wall = 0;
			}
			if (x == (n-1)) {
				f_neighbors--;
				r_wall = 0;
			}
			if (y == 0) {
				f_neighbors--;
				b_wall = 0;
			}
			if (y == (n-1)) {
				f_neighbors--;
				t_wall = 0;
			}
			if (x != 0) {
				b_neighbor = p[x-1][y];
			}
			if (y != 0) {
				l_neighbor = p[x][y-1];
			}
			
			node *n = malloc(sizeof(node));
			n->x = x;
			n->y = y;
			n->visited = 0;
			n->free_neighbors = f_neighbors;
			n->left_wall = l_wall;
			n->right_wall = r_wall;
			n->top_wall = t_wall;
			n->bottom_wall = b_wall;
			n->left_neighbor = l_neighbor;
			n->right_neighbor = r_neighbor;
			n->top_neighbor = t_neighbor;
			n->bottom_neighbor = b_neighbor;
			n->next = NULL;
			
			b_neighbor->top_neighbor = n;
			l_neighbor->right_neighbor = n;
		}
	}
}

node * get_next(node * n, int num) {
	if (n->free_neighbors == 0) {
		return NULL;
		}
	node *temp;
	if (num == 0) {
		temp = n->left_neighbor;
	}
	else if (num == 1) {
		temp = n->right_neighbor;
	}
	else if (num == 2) {
		temp = n->top_neighbor;
	}
	else {
		temp = n->bottom_neighbor;
	}
	
	if (temp->visited) {
		int next_num = (num+1) % 4;
		return get_next(n, next_num);
	}
	n->free_neighbors--;
	return temp;
}

void remove_wall(node *curr, node *next) {
	if (curr->left_neighbor == next) {
		curr->left_wall = 0;
		next->right_wall = 0;
	}
	else if (curr->right_neighbor == next) {
		curr->right_wall = 0;
		next->left_wall = 0;
	}
	else if (curr->top_neighbor == next) {
		curr->top_wall = 0;
		next->bottom_wall = 0;
	}
	else {
		curr->bottom_wall = 0;
		next->top_wall = 0;
	}
}

void push(node *n) {
	n->next = backtrack_queue;
	backtrack_queue = n;
}

node * pop() {
	node *temp = backtrack_queue;
	backtrack_queue = temp->next;
	return temp;
}

void gen_maze(node ***p) {
	node *start = p[0][0];
	node *next;
	int num = random(4);
	next = get_next(start, num);
	remove_wall(start, next);
	
	node * curr;
	curr = next;
	push(start);
	while(backtrack_queue != NULL) {
		num = random(4);
		next = get_next(curr, num);
		if (next == NULL) {
			curr = pop();
		}
		else {
			remove_wall(curr, next);
			curr = next;
		}
	}
}