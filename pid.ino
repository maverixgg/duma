void goOneCell2(){
  resetEncoder();
  // Reset PID on entry
  prevT = micros();
  eprev = 0;
  
  while (1) {
    // 1. Time calculation (non-blocking)
    long currT = micros();
    float deltaT = ((float)(currT - prevT)) / 1.0e6;
    prevT = currT;
    
    // 2. Safely read encoder (atomic)
    int currentLeftPos;
    int currentRightPos;
    noInterrupts();
    currentLeftPos = leftPos;
    currentRightPos = rightPos;
    interrupts();
    
    // Calculate average position (distance traveled)
    float avgPos = (currentLeftPos + currentRightPos) / 2.0;

    // 3. PID Calculations
    int error = targetDist - avgPos;

    // Ending Condition
    if(abs(error) < 5) {motorBrake(); break;}

    float dedt = deltaT > 0 ? (error - eprev) / deltaT : 0;

    // 4. Compute control signal
    float u = kp * error + kd * dedt;
    int pwr = constrain((int)u, 85, 200);
    eprev = error;

    // 5. Drive motors (assuming your motor() handles signed power)
    motor(pwr, pwr-2);
    
    // 6. Small yield instead of delay
    delayMicroseconds(500); // Allows ISRs to run
  }
  
  // Brake or coast when done
  motorBrake();
  motor(0,0);
  delay(500);
  resetEncoder();
}

void backError(){
  resetEncoder();
  motor(-150, -150);
  delay(backDelay);
  motorBrake();
  delay(100);
  resetEncoder();
}

void goStraight(){
  motor(254, 254);
  delay(straightDelay);
  motorBrake();
}