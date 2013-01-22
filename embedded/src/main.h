int main();

/**
 * Message: [GS]
 * Response: [FLRIIII]
 *   F: Front bumper event, _ioEventChar
 *   L: Left bumper event, _ioEventChar
 *   R: Right bumper event, _ioEventChar
 *   I: IR sensor value, hex encoded
 */
void getState();

/**
 * Message: [MVLLMRRS]
 * Response: [OK]
 *   L: Left target speed in rpms, hex-encoded
 *   M: Left direction, F for forward, R for reverse
 *   R: Right target speed in rpms, hex-encoded
 *   S: Right direction, F for forward, R for reverse
 */
void move();

/**
 * Send a char representing the IO event from the provided pin.
 *   L = LOW
 *   H = HIGH
 *   F = FALL
 *   R = RISE
 */
char _ioEvent(char pin);
