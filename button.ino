int button_read() {
  const int debounceTime = 15;     // Debounce time in ms
  const int timeout = 1000;        // Timeout to finish pressing sequence
  int pressCount = 0;

  unsigned long lastPressTime = 0;
  bool waitingForNext = false;

  while (true) {
    if (digitalRead(btn) == LOW) { // Button pressed (assuming active LOW)
      unsigned long pressStart = millis();

      // Wait until button is released
      while (digitalRead(btn) == LOW);

      unsigned long pressDuration = millis() - pressStart;

      // Valid press if longer than debounce time
      if (pressDuration >= debounceTime) {
        pressCount++;
        lastPressTime = millis();
        waitingForNext = true;
      }
    }

    if (waitingForNext) {
      // If user waits more than 1 second without pressing again
      if (millis() - lastPressTime > timeout) {
        return pressCount;
      }
    }
  }

  // Should never reach here, but good practice
  return pressCount;
}