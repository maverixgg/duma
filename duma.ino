// Sonar Declarations
#include <NewPing.h>

NewPing sonarF(13, 12, 40);
NewPing sonarL(18, 19, 40);
NewPing sonarR(14, 27, 40);

// Right Motor
#define lmf 15
#define lmb 2
#define lms 4

// Left Motor
#define rmf 25
#define rmb 33
#define rms 26

// Left Encoder
#define LENCA 39
#define LENCB 36

// Right Encoder
#define RENCA 35
#define RENCB 34

long prevT = 0;
float eprev = 0;
float eintegral = 0;

int target = 3000;
const uint16_t targetDiff = 1000; 
const uint16_t target180 = 2200; 


float kp = 1;
float ki = 0;
float kd = 0;

volatile long leftPos = 0;
volatile long rightPos = 0;

uint16_t wall = 15;

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define UNKNOWN 4
#define FLOOD_ONE_CELL 9050

// choose which algorithm to use in the beginning of the run
int algorithm;
struct dist_maze distances;
struct wall_maze cell_walls_info;
struct stack update_stack;
struct stack move_queue;

struct coor target;
init_coor(&target, 8, 7);

// to flood to center set third parameter to 1
init_distance_maze(&distances, &target, 1);

// initialize the walls
init_wall_maze(&cell_walls_info);

// set east, south, west wall of start cell to true
cell_walls_info.cells[0][0].walls[EAST] = 1;
cell_walls_info.cells[0][0].walls[SOUTH] = 1;
cell_walls_info.cells[0][0].walls[WEST] = 1;

struct coor c;
init_coor(&c, 0, 0);

int direction = NORTH;
update_stack.index = 0;


void setup() {

  Serial.begin(115200);

  //setup motor driver
  pinMode(lmf, OUTPUT);
  pinMode(lmb, OUTPUT);

  pinMode(rmf, OUTPUT);
  pinMode(rmb, OUTPUT);

  ledcAttach(rms, 10000, 8);
  ledcAttach(lms, 10000, 8);

  //Encoder Setup
  pinMode(LENCA, INPUT);
  pinMode(LENCB, INPUT);

  pinMode(RENCA, INPUT);
  pinMode(RENCB, INPUT);


  attachInterrupt(digitalPinToInterrupt(LENCA), readLeftEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(RENCA), readRightEncoder, RISING);

  delay(1500);

}

void loop() {
  direction = floodFill(&distances, &c, &cell_walls_info, 0, direction, &update_stack);
}
