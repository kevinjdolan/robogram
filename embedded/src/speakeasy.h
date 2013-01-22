#define MESSAGE_STATE_IDLE 0
#define MESSAGE_STATE_RECEIVING 1
#define MESSAGE_STATE_READY 2

#define MESSAGE_UNKNOWN 0
#define MESSAGE_GET_STATE 1
#define MESSAGE_MOVE 2

/**
 * Send the [OK] message.
 */
void acknowledgeMessage();

/**
 * Clear the current message.
 */
void clearMessage();

/**
 * End a message to the server.
 */
void endMessage();

/**
 * Return the character at the provided index from the current message.
 */
char getMessageChar(char index);

/**
 * Read a hex-encoded number from the message of the provided length.
 */
unsigned int getMessageNumber(char index, char length);

/**
 * Return the type of the message.
 */
char getMessageType();

/**
 * Check for data and add it to the buffer, possibly updating state.
 * Return 1 if a message available.
 */
char readMessage();

/**
 * Send some text to the server.
 */
void sendMessageChar(char txt);

/**
 * Send a hex-encoded number.
 * Will pad the number with '=' on the end up to length.
 * If the number is bigger than length, uhhh...
 */
void sendMessageNumber(unsigned int value, char length);

/**
 * Send some text to the server.
 */
void sendMessageString(char txt[]);

/**
 * Begin a message to the server.
 */
void startMessage();
