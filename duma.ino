#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define UNKNOWN 4
#define FLOOD_ONE_CELL 9050

// Sonar Declarations
#include <NewPing.h>
#include "maze_info.h"
#include "functions.h"

NewPing sonarF(13, 12, 200);
NewPing sonarL(18, 19, 200);
NewPing sonarR(14, 27, 200);

// Right Motor
#define lmf 15
#define lmb 2
#define lms 4

// Left Motor
#define rmb 25
#define rmf 33
#define rms 26

// Left Encoder
#define LENCA 39
#define LENCB 36

// Right Encoder
#define RENCA 35
#define RENCB 34

// LED
#define ledl 23
#define ledr 32
#define ledf 16

// Push Button
#define btn 17

long prevT = 0;
float eprev = 0;
float eintegral = 0;

const uint16_t targetDist = 907;
const uint16_t targetDiff = 990; 
const uint16_t target180 = 2100;

float kp = 1.2;
float ki = 0;
float kd = 0.08;

volatile long leftPos = 0;
volatile long rightPos = 0;

// PID variables for steering correction
float steerPrev = 0;

// PID gains for steering correction
float kp_steer = 2.0;
float kd_steer = 0.1;

uint16_t wall = 20;


// choose which algorithm to use in the beginning of the run
int algorithm;

struct dist_maze distances;
struct wall_maze cell_walls_info;
struct stack update_stack;
struct stack move_queue;

struct coor target;

struct coor c;

int direction = NORTH;

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

  //LED Setup
  pinMode(ledl, OUTPUT);
  pinMode(ledr, OUTPUT);
  pinMode(ledf, OUTPUT);

  //Push Button
  pinMode(btn, INPUT_PULLUP);

  update_stack.index = 0;

  init_coor(&target, 8, 7);

  // to flood to center set third parameter to 1
  init_distance_maze(&distances, &target, 1);

  // initialize the walls
  init_wall_maze(&cell_walls_info);

  // set east, south, west wall of start cell to true
  cell_walls_info.cells[0][0].walls[EAST] = 1;
  cell_walls_info.cells[0][0].walls[SOUTH] = 1;
  cell_walls_info.cells[0][0].walls[WEST] = 1;

  
  init_coor(&c, 0, 0);

  delay(1500);

}

void loop() {
  // direction = floodFill(&distances, &c, &cell_walls_info, 0, direction, &update_stack);
  // goOneCell();
  // if(sonarF_read()){
  //   while(1){
  //     motor(0,0);
  //   }
  // }
  // Serial.println(sonarF.ping_cm());

  Serial.print("L: ");
  Serial.print(leftPos);
  Serial.print("R: ");
  Serial.print(rightPos);
  Serial.println();
}
