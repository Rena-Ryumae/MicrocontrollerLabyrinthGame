#ifndef __MAZE_GEN__
#define __MAZE_GEN__

extern int n;

typedef struct maze_node {
	int x;
	int y;
	int visited;
	int free_neighbors;
	int left_wall;
	int right_wall;
	int top_wall;
	int bottom_wall;
	struct maze_node *left_neighbor;
	struct maze_node *right_neighbor;
	struct maze_node *top_neighbor;
	struct maze_node *bottom_neighbor;
	struct maze_node *next;
} node;

node ***init_board(int n);
	
void init_nodes(node*** p, int n);

void gen_maze(node ***p);

void free_all(node ***p);

#endif