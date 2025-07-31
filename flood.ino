int floodFill(struct dist_maze* dm, struct coor* c, struct wall_maze* wm, int a, int direction, struct stack* upst)
{
	// Disable tracking interrupts because we do not want to move yet
	// lockInterruptDisable_TIM3();
  noInterrupts();

	// coordinate for future use in popping stack
	int next_move;
	struct coor next;

  interrupts();
  
	// while we are not at target destination
	while(1)
	{
		// update coordinates for next cell we are going to visit
		switch(direction)
		{
		case NORTH: c->y += 1;
		break;
		case EAST: c->x += 1;
		break;
		case SOUTH: c->y -= 1;
		break;
		case WEST: c->x -= 1;
		break;
		default:
			break;
		}

		// If we haven't visited the next cell
		if(wm->cells[c->x][c->y].visited == 0)
		{
					// advance one cell
					advanceOneCell(direction, c, wm);

					// check for wall straight ahead
					if(sonarF_read())
					{
						// put wall ahead of us
						wm->cells[c->x][c->y].walls[direction] = 1;
						switch(direction)
						{
						// put wall ahead of us in the cell ahead of us
						case NORTH:
							if(c->y+1 < 16) wm->cells[c->x][c->y+1].walls[SOUTH] = 1;
							break;
						case EAST:
							if(c->x+1 < 16) wm->cells[c->x+1][c->y].walls[WEST] = 1;
							break;
						case SOUTH:
							if(c->y-1 > -1) wm->cells[c->x][c->y-1].walls[NORTH] = 1;
							break;
						case WEST:
							if(c->x-1 > -1) wm->cells[c->x-1][c->y].walls[EAST] = 1;
							break;
						}
					}

					// update the current cell as visited
					wm->cells[c->x][c->y].visited = 1;

		}
		else
		{
					// advance one cell without scanning for walls
					advanceOneCellVisited();
		}

		// if we are at target destination
		if (dm->distance[c->x][c->y]==0)
		{
			return direction;
			break;
		}

		// check if there is a neighbor with one less distance
		// next_move is the direction we should move next
		next_move = minusOneNeighbor(dm, wm, c, upst, a);

		// If we couldn't find a valid cell
		if(next_move == UNKNOWN)
		{
			// while stack is not empty
			while(upst->index!=0)
			{
				// get the cell to test from the stack
				next = pop_stack(upst);
				// find a neighbor cell with distance one less than current
				minusOneNeighbor(dm, wm, &next, upst, a);
			}
			// get next cell to traverse to
			// next_move is actually the direction we need to go next
			next_move = minusOneNeighbor(dm, wm, c, upst, a);
		}
		// Move to next cell
		// First turn to face the correct direction
		int difference = direction - next_move;
		switch(difference)
		{
		case -3:
			leftPivot();
			// calibration because left turn backs up mouse a little bit
			break;
		case -2:
			pivot180();
			// calibration by backing into wall behind us
			if(wm->cells[c->x][c->y].walls[direction]==1)
			{
				
			}
			break;
		case -1:
			rightPivot();
			break;
		case 0:
			break;
		case 1:
			leftPivot();
			uncontrolledAdvanceTicks(450);
			break;
		case 2:
			pivot180();
			if(wm->cells[c->x][c->y].walls[direction]==1)
			{

			}
			break;
		case 3:
			rightPivot();
			break;
		default:
			break;
		}

		// update the direction we are currently facing
		direction = next_move;
	}
}
