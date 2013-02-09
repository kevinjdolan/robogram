#include "settings.h"

#include <stdlib.h>

#include "bluetooth.h"
#include "speakeasy.h"

volatile char messageState = MESSAGE_STATE_IDLE;
volatile char buffer[SPEAKEASY_BUFFER_SIZE];
volatile char position = 0;
volatile char numBuffer[16];

void acknowledgeMessage() {
    startMessage();
    sendMessageString("OK");
    endMessage();
}

void clearMessage() {
    messageState = MESSAGE_STATE_IDLE;
}

void endMessage() {
    writeChar(']');
}

char getMessageChar(char index) {
    return buffer[2 + index];
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
    if(!strncmp(buffer, "GS", 2)) {
        return MESSAGE_GET_STATE;
    } 
    else if(!strncmp(buffer, "MV", 2)) {
        return MESSAGE_MOVE;
    }
    else {
        return MESSAGE_UNKNOWN;
    }
}

char readMessage() {
    char next = readAsync();
    
    if(messageState == MESSAGE_STATE_IDLE) {
        if(next == '[') {
            position = 0;
            messageState = MESSAGE_STATE_RECEIVING;
        }
    }
    else if(messageState == MESSAGE_STATE_RECEIVING) {
        if(next == ']') {
            messageState = MESSAGE_STATE_READY;
            buffer[position] = 0;
        }
        else if(next) {
            buffer[position] = next;
            position += 1;
        }
    }

    return messageState == MESSAGE_STATE_READY;
}

void sendMessageChar(char txt) {
    writeChar(txt);
}

void sendMessageString(char txt[]) {
    write(txt);
}

void sendMessageNumber(unsigned int value, char length) {
    itoa(value, numBuffer, 16);
    char count = 0;
    while(numBuffer[count]) {
        sendMessageChar(numBuffer[count]);
        count += 1;
    }
    for(;count < length; count++) {
        sendMessageChar('=');
    }
}

void startMessage() {
    writeChar('[');
}
