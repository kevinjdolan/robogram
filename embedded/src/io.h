/**
 * Convenient pin constants to be used by the functions defined here.
 */
#define B0 0
#define B1 1
#define B2 2
#define B3 3
#define B4 4
#define B5 5
#define B6 6
#define B7 7
#define C0 8
#define C1 9
#define C2 10
#define C3 11
#define C4 12
#define C5 13
#define C6 14
#define D0 16
#define D1 17
#define D2 18
#define D3 19
#define D4 20
#define D5 21
#define D6 22
#define D7 23

#define IO_LOW 0
#define IO_HIGH 1
#define IO_FALL 2
#define IO_RISE 3

/**
 * Initialize the state arrays for all pins.
 * This should be called after all input/output/high/low states have been set.
 */
void ioChangeInit();

/**
 * Enable the pin-change interrupt for the provided pin.
 * Pin changes can be discovered using the following vectors:
 *   B0-B7: PCINT0_vect
 *   C0-C6: PCINT1_vect
 *   D0-D7: PCINT2_vect
 */
 void ioInterrupt(char pin);

/**
 * Set the direction of the pin to output.
 */
void ioOutput(char pin);

/**
 * Set the OCRXX value corresponding to the pin.
 *
 * This is limited to B1, B2, B3, D2, D5, D6, and 
 * the correct timer setup must have been made in
 * order for this to work.
 */
void ioPower(char pin, unsigned short power);

/**
 * Return the current state of the pin, raw from the AVR.
 */
char ioRead(char pin);

/**
 * Return the debounced change-event of the pin, compared to its last read.
 *
 * Will return one of the following:
 *   IO_LOW
 *   IO_HIGH
 *   IO_FALL
 *   IO_RISE
 *
 * Note -- the accuracy of this method largely depends on not using ioRead directly
 * for the pin.
 */
char ioReadChange(char pin);

/**
 * Set the state of the pin to high if value, false otherwise.
 */
void ioWrite(char pin, char value);

/**
 * Set the state of the pin to high.
 */
void ioWriteHigh(char pin);

/**
 * Set the state of the pin to low.
 */
void ioWriteLow(char pin);
