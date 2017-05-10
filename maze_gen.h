#ifndef __MAZE_GEN__
#define __MAZE_GEN__

extern int n;

typedef struct final_state {
	int fx;
	int fy;
	int found;
} final;

int ** gen_maze(int n, final * f);

#endif