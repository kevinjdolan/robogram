#include "settings.h"

#include <stdlib.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "eyes.h"
#include "speakeasy.h"

volatile char readState = READ_STATE_IDLE;
volatile char sendState = SEND_STATE_IDLE;
volatile char readBuffer[SPEAKEASY_BUFFER_SIZE];
volatile char writeBuffer[SPEAKEASY_BUFFER_SIZE];
volatile char readPosition = 0;
volatile char writePosition = 0;
volatile char sendPosition = 0;
volatile char numBuffer[16];

char getMessageChar(char index) {
    return readBuffer[2 + index];
}

unsigned int getMessageNumber(char index, char length) {
    char i;
    unsigned int sum = 0;
    for(i = 0; i < length; i++) {
        char next = getMessageChar(i + index);
        if(next == '=') {
            break;
        }
        else {
            sum <<= 4;
            if(next >= 48 && next <= 57) {
                sum += next - 48; // 0 = 48 - 48 = 0
            }
            else if(next >= 65 && next <= 70) {
                sum += next - 55; // A = 97 - 87 = 10
            }
            else if(next >= 97 && next <= 102) {
                sum += next - 87; // a = 97 - 87 = 10
            }
            else {
                // ERROR!
            }
        }
    }
    return sum;
}

char getMessageType() {
    if(!strncmp(readBuffer, "GS", 2)) {
        return MESSAGE_GET_STATE;
    } 
    else if(!strncmp(readBuffer, "MV", 2)) {
        return MESSAGE_MOVE;
    }
    else {
        return MESSAGE_UNKNOWN;
    }
}

char readMessage() {
    if(readState == READ_STATE_READY) {
        return getMessageType();
    }
    else {
        return 0;
    }
}

void sendMessage() {
    sendState = SEND_STATE_START;
    sendPosition = 0;
    UCSR0B |= _BV(UDRIE0);
    
    // block until message has been sent
    while(sendState != SEND_STATE_IDLE) {}
    
    readState = READ_STATE_IDLE;
}

void speakeasyInit() {
    unsigned int baudrate = UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU);
    
    /* Set baud rate */
    if (baudrate & 0x8000) {
   		UCSR0A = (1 << U2X0);  //Enable 2x speed 
   		baudrate &= ~0x8000;
   	}
    UBRR0H = (unsigned char)(baudrate>>8);
    UBRR0L = (unsigned char) baudrate;

    /* Enable USART receiver and transmitter and receive complete interrupt */
    UCSR0B = _BV(RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
    
    /* Set frame format: asynchronous, 8data, no parity, 1stop bit */
    UCSR0C = (3<<UCSZ00);
}

void writeMessageChar(char txt) {
    writeBuffer[writePosition] = txt;
    writePosition += 1;
}

void writeMessageString(const char *s) {
    while (*s) {
      writeMessageChar(*s++);
    }
}

void writeMessageNumber(unsigned int value, char length) {
    itoa(value, numBuffer, 16);
    char count = 0;
    while(numBuffer[count]) {
        writeMessageChar(numBuffer[count]);
        count += 1;
    }
    for(;count < length; count++) {
        writeMessageChar('=');
    }
}

ISR(USART_RX_vect) {
    //Assume no error for now
    //unsigned char usr = UCSR0A;
    
    unsigned char next = UDR0;
    if(readState == READ_STATE_IDLE) {
        if(next == '[') {
            readPosition = 0;
            readState = READ_STATE_RECEIVING;
        }
    }
    else if(readState == READ_STATE_RECEIVING) {
        if(next == ']') {
            readState = READ_STATE_READY;
            writePosition = 0;
        }
        else if(next) {
            readBuffer[readPosition] = next;
            readPosition += 1;
        }
    }
}

ISR(USART_UDRE_vect) {
    if(sendState == SEND_STATE_START) {
        UDR0 = '[';
        sendState = SEND_STATE_SENDING;
    }
    else if(sendState == SEND_STATE_SENDING) {
        if(sendPosition < writePosition) {
            UDR0 = writeBuffer[sendPosition];
            sendPosition += 1;
        }
        else {
            UDR0 = ']';
            sendState = SEND_STATE_COMPLETE;
        }
    }
    else {
        UCSR0B &= ~_BV(UDRIE0);
        sendState = SEND_STATE_IDLE;
    }
}
