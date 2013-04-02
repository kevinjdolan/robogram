#include "settings.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include "eyes.h"
#include "io.h"
#include "main.h"
#include "motors.h"
#include "timers.h"
#include "sensors.h"
#include "speakeasy.h"

int main() {
    speakeasyInit();
    eyesInit();
    motorsInit();
    sensorsInit();
    ioChangeInit();
    
    sei();
    
    char messageType;
    while(1) {
        if(messageType = readMessage()) {
            if(messageType == MESSAGE_GET_STATE) {
                getState();
            }
            else if(messageType == MESSAGE_MOVE) {
                move();
            }
            sendMessage();
        }
    }
    return(0);
}

void getState() {
    _ioEvent(FRONT_BUMPER);
    _ioEvent(LEFT_BUMPER);
    _ioEvent(RIGHT_BUMPER);
    writeMessageNumber(getIrValue(), 4);
}

void move() {
    /*
    short leftSpeed = getMessageNumber(0, 2);
    if(getMessageChar(2) == 'R') {
        leftSpeed = -leftSpeed;
    }
    
    short rightSpeed = getMessageNumber(3, 2);
    if(getMessageChar(5) == 'R') {
        rightSpeed = -rightSpeed;
    }
    
    setWheelSpeeds(leftSpeed, rightSpeed);
    acknowledgeMessage();
    */
}

char _ioEvent(char pin) {
    char event = ioReadChange(pin);
    if(event == IO_LOW) {
        writeMessageChar('L');
    }
    else if(event == IO_HIGH) {
        writeMessageChar('H');
    }
    else if(event == IO_RISE) {
        writeMessageChar('R');
    }
    else { 
        writeMessageChar('F');
    }
}
