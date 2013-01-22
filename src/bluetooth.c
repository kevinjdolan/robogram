#include "settings.h"

#include <stdlib.h>

#include "uart.h"
#include "bluetooth.h"

char numBuffer[16];
unsigned int smallBuffer;

void bluetoothInit() {
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
}

char read() {
    while(1) {
        smallBuffer = uart_getc();
        if(!(smallBuffer & UART_NO_DATA)) {
            return (unsigned char) smallBuffer;
        }
    }
}

int readNumber() {
    char current = 0;
    while(1) {
        char next = read();
        if(next == 13) {
            numBuffer[current] = 0;
            writeln("");
            break;
        }
        else {
            numBuffer[current] = next;
            uart_putc(next);
        }
        current += 1;
    }
    return atoi(numBuffer);
}

void write(char txt[]) {
    uart_puts(txt);
}

void writeNumber(unsigned int number) {
    itoa(number, numBuffer, 10);
    write(numBuffer);
}

void writeln(char txt[]) {
    write(txt);
    write("\n\r");
}
