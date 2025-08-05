// Rotation PID Constants (tune these!)
const float rot_kp = 1;    // Start with these values
const float rot_ki = 0;
const float rot_kd = 0.07;

void leftPivot() {  
  // 2. Reset PID variables
  unsigned long rot_prevT = micros();
  int rot_eprev = 0;
  float rot_eintegral = 0;
  int leftStart = -leftPos;  // Capture initial positions
  int rightStart = rightPos;

  // 3. Rotation loop
  while (1) {
    // Safely read encoders
    int currentLeft, currentRight;
    noInterrupts();
    currentLeft = -leftPos;
    currentRight = rightPos;
    interrupts();
    
    // Calculate current rotation (difference in encoder counts)
    int actualDiff = (currentRight - rightStart) - (currentLeft - leftStart);
    Serial.print(actualDiff);

    // Exit condition (5-count tolerance)
    if (abs(actualDiff - targetDiff) < 30) break;

    // PID Calculations
    long currT = micros();
    float deltaT = ((float)(currT - rot_prevT)) / 1.0e6;
    rot_prevT = currT;
    
    // Error Calculation
    int error = targetDiff - actualDiff;

    float dedt = deltaT > 0 ? (error - rot_eprev) / deltaT : 0;
    rot_eintegral += error * deltaT;
    rot_eintegral = constrain(rot_eintegral, -50, 50); // Anti-windup
    
    float u = rot_kp * error + rot_kd * dedt + rot_ki * rot_eintegral;
    int pwr = constrain((int)u, -255, 255);
    rot_eprev = error;

    // Drive motors in opposite directions
    motor(-pwr, pwr);
    
    delayMicroseconds(500); // Allow ISRs to run
  }
  
  motor(0, 0); // Brake
  delay(500);
  resetEncoder();
}

void rightPivot() { 
  // 2. Reset PID variables
  unsigned long rot_prevT = micros();
  int rot_eprev = 0;
  float rot_eintegral = 0;
  int leftStart = leftPos;  // Capture initial positions
  int rightStart = rightPos;

  // 3. Rotation loop
  while (1) {
    // Safely read encoders
    int currentLeft, currentRight;
    noInterrupts();
    currentLeft = leftPos;
    currentRight = rightPos;
    interrupts();
    
    // Calculate current rotation (difference in encoder counts)
    int actualDiff = (currentRight - rightStart) - (currentLeft - leftStart);

    // Exit condition (5-count tolerance)
    if (abs(targetDiff + actualDiff) < 30) {motor(0,0);delay(1000); resetEncoder(); return;}
    
    // PID Calculations
    long currT = micros();
    float deltaT = ((float)(currT - rot_prevT)) / 1.0e6;
    rot_prevT = currT;
    
    // Error Calculation
    int error = targetDiff + actualDiff;

    float dedt = deltaT > 0 ? (error - rot_eprev) / deltaT : 0;
    rot_eintegral += error * deltaT;
    rot_eintegral = constrain(rot_eintegral, -50, 50); // Anti-windup
    
    float u = rot_kp * error + rot_kd * dedt + rot_ki * rot_eintegral;
    int pwr = constrain((int)u, -255, 255);
    rot_eprev = error;
    
    // Drive motors in opposite directions
    motor(pwr, -pwr);
    
    delayMicroseconds(500); // Allow ISRs to run
  }
  
  motor(0, 0); // Brake
  delay(500);
  resetEncoder();
  return;
}

void pivot180(){
  // 2. Reset PID variables
  unsigned long rot_prevT = micros();
  int rot_eprev = 0;
  float rot_eintegral = 0;
  int leftStart = -leftPos;  // Capture initial positions
  int rightStart = rightPos;

  // 3. Rotation loop
  while (1) {
    // Safely read encoders
    int currentLeft, currentRight;
    noInterrupts();
    currentLeft = -leftPos;
    currentRight = rightPos;
    interrupts();
    
    // Calculate current rotation (difference in encoder counts)
    int actualDiff = (currentRight - rightStart) - (currentLeft - leftStart);
    Serial.print(actualDiff);

    // Exit condition (5-count tolerance)
    if (abs(actualDiff - target180) < 30) break;

    // PID Calculations
    long currT = micros();
    float deltaT = ((float)(currT - rot_prevT)) / 1.0e6;
    rot_prevT = currT;
    
    // Error Calculation
    int error = target180 - actualDiff;

    float dedt = deltaT > 0 ? (error - rot_eprev) / deltaT : 0;
    rot_eintegral += error * deltaT;
    rot_eintegral = constrain(rot_eintegral, -50, 50); // Anti-windup
    
    float u = rot_kp * error + rot_kd * dedt + rot_ki * rot_eintegral;
    int pwr = constrain((int)u, -255, 255);
    rot_eprev = error;

    // Drive motors in opposite directions
    motor(-pwr, pwr);
    
    delayMicroseconds(500); // Allow ISRs to run
  }
  
  motor(0, 0); // Brake
  delay(500);
  resetEncoder();
}