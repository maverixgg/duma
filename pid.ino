void goOneCell(){
  // Reset PID on entry
  prevT = micros();
  eprev = 0;
  eintegral = 0;
  
  while (abs(target - rightPos) > 5) {
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
    int error = target - currentPos;
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
  resetEncoder();
}