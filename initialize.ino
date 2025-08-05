#include "maze_info.h"

void init_distance_maze(struct dist_maze* dm, struct coor* c, int center)
{
	// If we are trying to get to the center
	// Initialize the distance maze with the center four cells as zero
	if(center == 1)
	{
		for(int i=0; i<16; i++)
		{
			for(int j=0; j<16; j++)
			{
				if(i<=7 && j<=7) dm->distance[i][j] = ((7-i) + (7-j));
				if(i<=7 && j>7) dm->distance[i][j] = ((7-i) + (j-8));
				if(i>7 && j<=7) dm->distance[i][j] = ((i-8) + (7-j));
				if(i>7 && j>7) dm->distance[i][j] = ((i-8) + (j-8));
			}
		}
	}
	// Generalized floodfill for any other cell
	else
	{
		for(int i=0; i<16; i++)
		{
			for(int j=0; j<16; j++)
			{
				// get the distance to target cell
				int x = i - c->x;
				if(x<0) x = -x;
				int y = j - c->y;
				if(y<0) y = -y;
				dm->distance[i][j] = x + y;
			}
		}
	}
}

// Initialize all cells to unvisited
void init_wall_maze(struct wall_maze* wm)
{
	for(int i=0; i<16 ; i++)
	{
		for(int j=0; j<16 ; j++)
		{
			wm->cells[i][j].walls[NORTH] = 0;
			wm->cells[i][j].walls[EAST] = 0;
			wm->cells[i][j].walls[SOUTH] = 0;
			wm->cells[i][j].walls[WEST] = 0;
			wm->cells[i][j].visited = 0;
			// put the walls surrounding the maze
			if(i==0) wm->cells[i][j].walls[WEST] = 1;
			if(j==0) wm->cells[i][j].walls[SOUTH] = 1;
			if(i==15) wm->cells[i][j].walls[EAST] = 1;
			if(j==15) wm->cells[i][j].walls[NORTH] = 1;
		}
	}
}

// Constructor for initializing coordinates
void init_coor(struct coor* c, int x, int y){
	c->x = x;
	c->y = y;
}

// Pop top of stack
struct coor pop_stack(struct stack* s){
	s->index = s->index -1;
	return s->array[s->index + 1];
}

// Push to top of stack
void push_stack(struct stack* s, struct coor c){
	s->index = s->index +1;
	s->array[s->index] = c;
}
