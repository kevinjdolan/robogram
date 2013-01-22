#define FRONT_BUMPER B1
#define LEFT_BUMPER D3
#define RIGHT_BUMPER D2

/**
 * Initialize the sensors.
 */
void sensorsInit();

/**
 * Return the value of the ir sensor.
 */
unsigned int getIrValue();
