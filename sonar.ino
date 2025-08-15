bool sonarF_read() {
  sonarF.ping_cm();
  sonarF.ping_cm();

  uint16_t distance = sonarF.ping_cm();
  if (distance <= 0) distance = 50;

  return distance < 13;
}

bool sonarR_read() {
  sonarR.ping_cm();
  sonarR.ping_cm();

  uint16_t distance = sonarR.ping_cm();
  if (distance <= 1) distance = 50;

  return distance < wall;
}

bool sonarL_read() {
  sonarL.ping_cm();
  sonarL.ping_cm();

  uint16_t distance = sonarL.ping_cm();
  if (distance <= 1) distance = 50;

  return distance < wall;
}

void frontCalibration(){
  if(sonarF_read()){
    int dist = sonarF.ping_cm();

    while(dist < 2 || dist > 3) {
      if(dist > 3){
        motor(80,80);
        delay(100);
        motor(0,0);
        delayMicroseconds(250);
      }

      else if(dist < 2) {
        motor(-80,-80);
        delay(100);
        motor(0,0);
        delayMicroseconds(250);
      }
      dist = sonarF.ping_cm();
    }
  }
  else {
    motor(0,0);
  }
  motor(0,0);
  resetEncoder();
  delay(500);
}