#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3
#define UNKNOWN 4
#define FLOOD_ONE_CELL 9050

// Sonar Declarations
#include <NewPing.h>
#include <Arduino.h>
#include <TinyMPU6050.h>
#include "maze_info.h"
#include "functions.h"

MPU6050 mpu (Wire);

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

// Push Button
#define btn 17

int straightDelay = 800;
int pidError = 4;

long prevT = 0;
float eprev = 0;

uint16_t targetDist = 690;
int backTarget = -250;
const uint16_t targetDiff = 990;
const uint16_t target180 = 2200;

float rightangle = 97;
float leftangle = 89.0;

float kp = 1.5;
float kd = 0.1;

volatile long leftPos = 0;
volatile long rightPos = 0;

uint16_t wall = 15;

uint16_t backDelay = 185;

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

  mpu.Initialize();
  // mpu.Calibrate();

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

  delay(500);
}

void loop() {
  int r = button_read();
  if(r == 1) floodFill(&distances, &c, &cell_walls_info, 0, direction, &update_stack);
  if(r == 2) while(1) {leftWallHugger();};
  if(r == 3) while(1) {rightWallHugger();};
  if(r == 4) goOneCell2();
  if(r == 5) goStraight();
  if(r == 6) turnRight(rightangle);
  if(r == 7) turnLeft(leftangle);
  if(r == 8) turn180();
  if(r == 9) targetDist += 15;
  if(r == 10) targetDist -= 15;
  if(r == 11) rightangle += 1;
  if(r == 12) rightangle -= 1;
}
