void turnRight(float rightangle){
  float kp = 2.75;
  float kd = 0.01;

  prevT = micros();
  eprev = 0;

  resetEncoder();
  mpu.Execute();
  float startAngle = mpu.GetAngZ();
  // float targetAngle = 90 + startAngle;

  while(1) {
    long currT = micros();
    float deltaT = ((float)(currT - prevT)) / 1.0e6;
    prevT = currT;

    mpu.Execute(); // Update sensor readings
    
    // Check if we've reached target
    float currentAng = mpu.GetAngZ();
    if(startAngle >= 90 && startAngle < 180){
      if(currentAng<0) currentAng += 360;
    }

    if(fabs(currentAng - startAngle) > rightangle) {
      motorBrake();
      break;
    }

    float error = 90 - fabs(currentAng - startAngle);
    float dedt = deltaT > 0 ? (error - eprev) / deltaT : 0;
    eprev = error;

    float u = kp * error + kd * dedt;
    int pwr = constrain((int)u, 85, 230);

    motor(pwr, -pwr);

    delay(3);
  }
  
  // Stop motors
  // motor(0, 0);
  delay(500);
  resetEncoder();
}

void turnLeft(float leftangle){
  resetEncoder();
  
  float kp = 2.75;
  float kd = 0.025;

  prevT = micros();
  eprev = 0;
  mpu.Execute();
  float startAngle = mpu.GetAngZ();
  // float targetAngle = 90 + startAngle;

  while(1) {
    long currT = micros();
    float deltaT = ((float)(currT - prevT)) / 1.0e6;
    prevT = currT;

    mpu.Execute(); // Update sensor readings
    
    // Check if we've reached target
    float currentAng = mpu.GetAngZ();
    if(startAngle <= -90 && startAngle > -180){
      if(currentAng>=0) currentAng -= 360;
    }

    if(fabs(currentAng - startAngle) > leftangle) {
      motorBrake();
      break;
    }

    float error = 90 - fabs(currentAng - startAngle);
    float dedt = deltaT > 0 ? (error - eprev) / deltaT : 0;
    eprev = error;

    float u = kp * error + kd * dedt;
    int pwr = constrain((int)u, 85, 230);

    motor(-pwr, pwr);

    delay(3);
  }

  // Stop motors
  motor(0, 0);
  delay(500);
  resetEncoder();
}

void turn180(){
  float kp = 2.75;
  float kd = 0.035;

  prevT = micros();
  eprev = 0;
  resetEncoder();
  mpu.Execute();
  float startAngle = mpu.GetAngZ();
  // float targetAngle = 90 + startAngle;

  while(1) {
    long currT = micros();
    float deltaT = ((float)(currT - prevT)) / 1.0e6;
    prevT = currT;

    mpu.Execute(); // Update sensor readings
    
    // Check if we've reached target
    float currentAng = mpu.GetAngZ();
    if(startAngle < 0){
      if(currentAng>=0) currentAng -= 360;
    }

    if(fabs(currentAng - startAngle) > 178.0) {
      motorBrake();
      motorBrake();
      break;
    }

    float error = 180 - fabs(currentAng - startAngle);
    float dedt = deltaT > 0 ? (error - eprev) / deltaT : 0;
    eprev = error;

    float u = kp * error + kd * dedt;
    int pwr = constrain((int)u, 85, 220);

    motor(-pwr, pwr);

    delayMicroseconds(3000);
  }

  // Stop motors
  motorBrake();
  delay(500);
  resetEncoder();
}