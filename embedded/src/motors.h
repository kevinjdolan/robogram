#define LEFT_POWER D6
#define RIGHT_POWER D5
#define LEFT_DIRECTION B0
#define RIGHT_DIRECTION D7
#define LEFT_ENCODER C1
#define RIGHT_ENCODER C5

#define WHEEL_ENCODER_SAMPLE_COUNT 4
#define WHEEL_ENCODER_CLICKS_PER_REVOLUTION 12

#define FORWARD 1
#define REVERSE 0

#define LEFT_WHEEL 0
#define RIGHT_WHEEL 1

/**
 * The struct in which to place information about the Wheel.
 */
typedef struct {
    char encoderPin;
    char powerPin;
    char directionPin;
    volatile char state;
    volatile char risen;
    volatile unsigned int time;
    volatile unsigned short samples[WHEEL_ENCODER_SAMPLE_COUNT];
    volatile unsigned char next;
    volatile short previousError;
    volatile int errorIntegral;
    volatile unsigned short targetRpm;
    volatile unsigned int power;
    volatile unsigned int count;
} WheelState;

/**
 * Return the count of the wheel since the last reset.
 */
unsigned int getWheelCount(char wheel);

/**
 * Initialize motors with the following arrangement:
 *   PB0: left motor direction, 0 = backwards, 1 = forward
 *   PD7: right motor direction, 0 = backwards, 1 = forward
 *   PD6 (OCR0A): left motor direction
 *   PD5 (OCR0B): right motor direction
 */
void motorsInit();

/**
 * Call set wheel speed with the appropriate wheel.
 */
void setWheelSpeed(char wheel, short speed);

/**
 * Set the motor speeds for the left and right together.
 */
void setWheelSpeeds(short leftSpeed, short rightSpeed);

/**
 * Return the computed rpm from the provided samples.
 */
unsigned short _getSampledRpm(WheelState *state);

/**
 * Return the wheel state for the appropriate wheel.
 */
WheelState *_getWheelState(char wheel);

/**
 * The abstracted read method to be used for each wheel state.
 */
void _readWheelState(WheelState *state);

/**
 * Reset the wheel state struct to begin accepting new data.
 */
void _resetWheelState(WheelState *state, unsigned short targetRpm, char direction);

/**
 * Perform a single iteration of the PID control.
 */
void _updatePid(WheelState *state, unsigned int dt);

/**
 * Set the power level for the given wheel.
 */
void _updatePower(WheelState *state);

/**
 * Set the speed (in rpm) of the provided motor state.
 * Negative values should indicate reverse.
 */
void _setWheelSpeed(WheelState *state, short speed);
