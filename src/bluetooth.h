/**
 * Start the bluetooth nonsense.
 */
void bluetoothInit();

/**
 * Synchronously read a single character, blocks until a character is read.
 */
char read();

/**
 * Read in an integer.
 */
int readNumber();

/**
 * Write a string.
 */
void write(char txt[]);

/**
 * Write a number to bluetooth.
 */
void writeNumber(unsigned int number);

/**
 * Write a string, followed by a newline.
 */
void writeln(char txt[]);
