#include "settings.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include "bluetooth.h"
#include "eyes.h"
#include "io.h"
#include "main.h"
#include "motors.h"
#include "timers.h"
#include "sensors.h"
#include "speakeasy.h"

int main() {
    bluetoothInit();
    eyesInit();
    motorsInit();
    sensorsInit();
    ioChangeInit();
    
    sei();

    writeln("-- BEGIN --");
    while(1) {
        if(readMessage()) {
            writeln("");
            char type = getMessageType();
            if(type == MESSAGE_GET_STATE) {
                getState();
            }
            else if(type == MESSAGE_MOVE) {
                move();
            }
            clearMessage();
            writeln("");
        }
    }
    return(0);
}

void getState() {
    startMessage();
    _ioEvent(FRONT_BUMPER);
    _ioEvent(LEFT_BUMPER);
    _ioEvent(RIGHT_BUMPER);
    sendMessageNumber(getIrValue(), 4);
    endMessage();
}

void move() {
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
}

char _ioEvent(char pin) {
    char event = ioReadChange(pin);
    if(event == IO_LOW) {
        sendMessageChar('L');
    }
    else if(event == IO_HIGH) {
        sendMessageChar('H');
    }
    else if(event == IO_RISE) {
        sendMessageChar('R');
    }
    else { 
        sendMessageChar('F');
    }
}
