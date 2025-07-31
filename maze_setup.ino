// struct to hold distance info
struct dist_maze{
    int distance[16][16];
};

// define structures needed for flood fill
struct cell_info{
	// variables for north,east,south,west walls
	bool walls[4];
	bool visited;
};

// struct to hold cell info
struct wall_maze{
	struct cell_info cells[16][16];
};

// struct to hold coordinates
struct coor{
	int x;
	int y;
};

// stack for iterative floodfill
struct stack{
	struct coor array[256];
	int index;
};

