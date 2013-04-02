#define READ_STATE_IDLE 0
#define READ_STATE_RECEIVING 1
#define READ_STATE_READY 2

#define MESSAGE_UNKNOWN 1
#define MESSAGE_GET_STATE 2
#define MESSAGE_MOVE 3

#define SEND_STATE_IDLE 0
#define SEND_STATE_START 1
#define SEND_STATE_SENDING 2
#define SEND_STATE_COMPLETE 3

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
 * Return message type if a message is available, 0 if not.
 */
char readMessage();


/**
 * Initialize to receive bluetooth messages.
 */
void speakeasyInit();

/**
 * Send the pending message.
 */
void sendMessage();

/**
 * Add a character to be sent to the server.
 */
void writeMessageChar(char txt);

/**
 * Add a hex-encoded number to be sent to the server.
 * Will pad the number with '=' on the end up to length.
 * If the number is bigger than length, uhhh...
 */
void writeMessageNumber(unsigned int value, char length);

/**
 * Add a string to be sent to the server.
 */
void writeMessageString(const char *s);

