void goOneCell(){
  // Reset PID on entry
  prevT = micros();
  eprev = 0;
  eintegral = 0;
  
  while (abs(targetDist - rightPos) > 5) {
    // 1. Time calculation (non-blocking)
    long currT = micros();
    float deltaT = ((float)(currT - prevT)) / 1.0e6;
    prevT = currT;
    
    // 2. Safely read encoder (atomic)
    int currentPos;
    noInterrupts();
    currentPos = rightPos;
    interrupts();
    
    // 3. PID Calculations
    int error = targetDist - currentPos;
    float dedt = deltaT > 0 ? (error - eprev) / deltaT : 0;
    eintegral += error * deltaT;
    
    // Anti-windup: Clamp integral term
    eintegral = constrain(eintegral, -100, 100); // Adjust bounds as needed
    
    // 4. Compute control signal
    float u = kp * error + kd * dedt + ki * eintegral;
    int pwr = constrain((int)u, -255, 255);
    eprev = error;
    
    // 5. Drive motors (assuming your motor() handles signed power)
    motor(pwr, pwr);
    
    // 6. Small yield instead of delay
    delayMicroseconds(500); // Allows ISRs to run
  }
  
  // Brake or coast when done
  motor(0, 0);
  delay(1000);
  resetEncoder();
}

void goOneCell2(){
  // Reset PID on entry
  prevT = micros();
  eprev = 0;
  eintegral = 0;
  
  while (abs(targetDist - rightPos) > 5) {
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
    float avgPos = (leftPos + rightPos) / 2.0;

    // Calculate steering error (difference between wheels)
    float steerError = leftPos - rightPos;

    // 3. PID Calculations
    int error = targetDist - avgPos;
    float dedt = deltaT > 0 ? (error - eprev) / deltaT : 0;
    eintegral += error * deltaT;
    
    // Anti-windup: Clamp integral term
    eintegral = constrain(eintegral, -100, 100); // Adjust bounds as needed

    // Steering PID control (to keep robot straight)
    float steerDedt = (steerError - steerPrev) / deltaT;
    
    // Calculate steering correction
    float steerCorrection = kp_steer * steerError + kd_steer * steerDedt;
  
    // 4. Compute control signal
    float u = kp * error + kd * dedt + ki * eintegral;
    int pwr = constrain((int)u, -255, 255);
    eprev = error;

    // Calculate motor speeds
    float leftSpeed = pwr - steerCorrection;
    float rightSpeed = pwr + steerCorrection;
    
    // 5. Drive motors (assuming your motor() handles signed power)
    motor(leftSpeed, rightSpeed);
    
    // 6. Small yield instead of delay
    delayMicroseconds(500); // Allows ISRs to run
  }
  
  // Brake or coast when done
  motor(0, 0);
  delay(1000);
  resetEncoder();
}
