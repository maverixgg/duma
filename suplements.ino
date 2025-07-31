// move one cell forward and also poll adc for wall values
void advanceOneCell(int d, struct coor* c, struct wall_maze* wm) {
	resetEncoder();
	// put walls up for the cell we are moving to. For each direction put a wall to the
	// east and the west.
	switch(d)
	{
	case NORTH:
		wm->cells[c->x][c->y].walls[WEST] = 1;
		wm->cells[c->x][c->y].walls[EAST] = 1;
		// checks for out of bounds
		if(c->x > 0) wm->cells[c->x-1][c->y].walls[EAST] = 1;
		if(c->x < 15) wm->cells[c->x+1][c->y].walls[WEST] = 1;
		break;
	case EAST:
		wm->cells[c->x][c->y].walls[NORTH] = 1;
		wm->cells[c->x][c->y].walls[SOUTH] = 1;
		if(c->y > 0) wm->cells[c->x][c->y-1].walls[NORTH] = 1;
		if(c->y < 15) wm->cells[c->x][c->y+1].walls[SOUTH] = 1;
		break;
	case SOUTH:
		wm->cells[c->x][c->y].walls[WEST] = 1;
		wm->cells[c->x][c->y].walls[EAST] = 1;
		if(c->x > 0) wm->cells[c->x-1][c->y].walls[EAST] = 1;
		if(c->x < 15) wm->cells[c->x+1][c->y].walls[WEST] = 1;
		break;
	case WEST:
		wm->cells[c->x][c->y].walls[NORTH] = 1;
		wm->cells[c->x][c->y].walls[SOUTH] = 1;
		if(c->y > 0) wm->cells[c->x][c->y-1].walls[NORTH] = 1;
		if(c->y < 15) wm->cells[c->x][c->y+1].walls[SOUTH] = 1;
		break;
	default:
		break;
	}
	
  goOneCell();

			// check for walls to the east and the west
			switch(d)
			{
			case NORTH: checkForWalls(wm, c, d, NORTH, EAST, SOUTH, WEST);
			break;
			case EAST: checkForWalls(wm, c, d, EAST, SOUTH, WEST, NORTH);
			break;
			case SOUTH: checkForWalls(wm, c, d, SOUTH, WEST, NORTH, EAST);
			break;
			case WEST: checkForWalls(wm, c, d, WEST, NORTH, EAST, SOUTH);
			break;
			default:
			break;
			}
		}
	}
	resetEncoder();
}


void checkForWalls(struct wall_maze* wm, struct coor* c, int direction, int n, int e, int s, int w)
{
	// if there is a wall in memory
	if(wm->cells[c->x][c->y].walls[w]==1)
	{
		// check for wall to the west
		// if at anytime the value drops below that means there is no wall
		if(!sonarL_read())
		{
			wm->cells[c->x][c->y].walls[w] = 0;
			switch(direction)
			{
			// also put walls in the other adjacent cells
			case NORTH:
				if(c->x-1 > -1) wm->cells[c->x-1][c->y].walls[EAST] = 0;
				break;
			case EAST:
				if(c->y+1 < 16) wm->cells[c->x][c->y+1].walls[SOUTH] = 0;
				break;
			case SOUTH:
				if(c->x+1 < 16)wm->cells[c->x+1][c->y].walls[WEST] = 0;
				break;
			case WEST:
				if(c->y-1 > -1) wm->cells[c->x][c->y-1].walls[NORTH] = 0;
				break;
			}
		}
	}

	if(wm->cells[c->x][c->y].walls[e]==1)
	{
		// check for wall to the east
		if(!sonarR_read())
		{
			wm->cells[c->x][c->y].walls[e] = 0;
			switch(direction)
			{
			case NORTH:
				if(c->x+1 < 16)wm->cells[c->x+1][c->y].walls[WEST] = 0;
				break;
			case EAST:
				if(c->y-1 > -1) wm->cells[c->x][c->y-1].walls[NORTH] = 0;
				break;
			case SOUTH:
				if(c->x-1 > -1) wm->cells[c->x-1][c->y].walls[EAST] = 0;
				break;
			case WEST:
				if(c->y+1 < 16) wm->cells[c->x][c->y+1].walls[SOUTH] = 0;
				break;
			}
		}
	}
}


int minusOneNeighbor(struct dist_maze* dm, struct wall_maze* wm, struct coor* c, struct stack* s, int a)
{
	int i;
	// minimum distance
	int md=260;
	// get target distance we're looking for
	int target = dm->distance[c->x][c->y] - 1;
	// check neighbor cells
	for(i=0; i<4; i++)
	{
		// choice of direction preference
		int j = (i + a) % 4;
		// If there is no wall blocking the way
		if(wm->cells[c->x][c->y].walls[j]==0)
		{
			switch(j)
			{
			case NORTH:
				if(dm->distance[c->x][c->y+1]==target)
				{
					// if the cell exists return the direction we want to move
					return j;
				}
				if(dm->distance[c->x][c->y+1] < md) md = dm->distance[c->x][c->y+1];
				break;
			case EAST:
				if(dm->distance[c->x+1][c->y]==target)
				{
					// if the cell exists return the direction we want to move
					return j;
				}
				if(dm->distance[c->x+1][c->y] < md) md = dm->distance[c->x+1][c->y];
				break;
			case SOUTH:
				if(dm->distance[c->x][c->y-1]==target)
				{
					// if the cell exists return the direction we want to move
					return j;
				}
				if(dm->distance[c->x][c->y-1] < md) md = dm->distance[c->x][c->y-1];
				break;
			case WEST:
				if(dm->distance[c->x-1][c->y]==target)
				{
					// if the cell exists return the direction we want to move
					return j;
				}
				if(dm->distance[c->x-1][c->y] < md) md = dm->distance[c->x-1][c->y];
				break;
			default:

				break;
			}
		}
	}

	// update distance of coordinate to 1 plus minimum distance
	dm->distance[c->x][c->y] = md + 1;

	// Since we did not find a cell we push onto the stack
	for(i=0; i<4; i++)
	{
		// choice of direction preference
		int j = (i + a) % 4;
		// If there is no wall blocking the way
		if(wm->cells[c->x][c->y].walls[j]==0)
		{
			struct coor temp;
			switch(j)
			{
			case NORTH:
				init_coor(&temp, c->x, c->y + 1);
				break;
			case EAST:
				init_coor(&temp, c->x + 1, c->y);
				break;
			case SOUTH:
				init_coor(&temp, c->x, c->y - 1);
				break;
			case WEST:
				init_coor(&temp, c->x - 1, c->y);
				break;
			}
			push_stack(s, temp);
		}
	}
	// return unknown
	return UNKNOWN;
}

