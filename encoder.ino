void resetEncoder(){
  rightPos = 0;
  leftPos = 0;
}

void IRAM_ATTR readLeftEncoder() {
  int b = digitalRead(LENCB);
  if (b > 0) {
    leftPos++;
  } else {
    leftPos--;
  }
}

void IRAM_ATTR readRightEncoder() {
  int b = digitalRead(RENCB);
  if (b > 0) {
    rightPos--;  // Note: might need to reverse this depending on motor orientation
  } else {
    rightPos++;
  }
}
