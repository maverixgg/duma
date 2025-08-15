void leftWallHugger()
{
  bool rightBlocked = sonarR_read(); // true = wall
  bool frontBlocked = sonarF_read();
  bool leftBlocked  = sonarL_read();

	if (!leftBlocked)
	{
		frontCalibration();        // optional: only if you want a crisp bumper align
    turnLeft(leftangle);
    backError();               // if this is your “snap to cell center”
    goOneCell2();
    return;
	}

  // 2) Go straight if front open
  if (!frontBlocked) {
    goOneCell2();
    return;
  }

  // 3) Otherwise, if right open, turn right
  if (!rightBlocked) {
    frontCalibration();
    turnRight(rightangle);
    backError();
    goOneCell2();
    return;
  }

  // 4) Dead end: U-turn
  // If you truly want to “advance further into cell to do the 180”,
  // put a short forward move *before* the turn.
  // goOneCell2();  // (tiny fraction, not a full cell)
  turn180();
  backError();
  goOneCell2();
}
