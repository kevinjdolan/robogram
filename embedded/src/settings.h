#define F_CPU 14745600UL

#define UART_BAUD_RATE 288000
#define UART_BAUD_SELECT(baudRate,xtalCpu)  (((xtalCpu) + 8UL * (baudRate)) / (16UL * (baudRate)) -1UL)

#define DEBOUNCE_TIME 500

#define SPEAKEASY_BUFFER_SIZE 32
