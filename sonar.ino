bool sonarF_read() {
    uint16_t distance = sonarF.ping_cm();
    if (distance <= 2) distance = 50;

  if (distance < wall) return 1;
  else return 0;
}

bool sonarR_read() {
    uint16_t distance = sonarR.ping_cm();
    if (distance <= 2) distance = 50;

  if (distance < wall) return 1;
  else return 0;
}

bool sonarL_read() {
    uint16_t distance = sonarL.ping_cm();
    if (distance <= 2) distance = 50;

  if (distance < wall) return 1;
  else return 0;
}