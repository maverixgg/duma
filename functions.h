#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "maze_info.h"

int floodFill(struct dist_maze* dm, struct coor* c, struct wall_maze* wm, int a, int direction, struct stack* upst);
void init_distance_maze(struct dist_maze* dm, struct coor* c, int center);
void init_wall_maze(struct wall_maze* wm);
void init_coor(struct coor* c, int x, int y);
struct coor pop_stack(struct stack* s);
void push_stack(struct stack* s, struct coor c);
void advanceOneCell(int d, struct coor* c, struct wall_maze* wm);
void checkForWalls(struct wall_maze* wm, struct coor* c, int direction, int n, int e, int s, int w);
int minusOneNeighbor(struct dist_maze* dm, struct wall_maze* wm, struct coor* c, struct stack* s, int a);
void leftPivot();
void rightPivot();
void pivot180();
void resetEncoder();
void IRAM_ATTR readLeftEncoder();
void IRAM_ATTR readRightEncoder();

#endif
