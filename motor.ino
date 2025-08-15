void  motor(int a, int b){
  if(a>0){
    digitalWrite(lmf, 1);
    digitalWrite(lmb, 0);
  }
  else{
    a = -(a);
    digitalWrite(lmf, 0);
    digitalWrite(lmb, 1);
  }

  if(b>0){
    digitalWrite(rmf, 1);
    digitalWrite(rmb, 0);
  }
  else{
    b = -(b);
    digitalWrite(rmf, 0);
    digitalWrite(rmb, 1);
  }

  ledcWrite(rms, b);
  ledcWrite(lms, a);
}

void motorBrake() {
  // Left short brake
  digitalWrite(lmf, LOW);
  digitalWrite(lmb, LOW);
  ledcWrite(lms, 255);   // brief full brake
  // Right short brake
  digitalWrite(rmf, LOW);
  digitalWrite(rmb, LOW);
  ledcWrite(rms, 255);
  delay(20);             // 10–40 ms tune

  // Release to hold (no torque)
  ledcWrite(lms, 0);
  ledcWrite(rms, 0);
}