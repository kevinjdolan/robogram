#include "settings.h"

#include <util/delay.h>

#import "io.h"
#import "timers.h"

void eyesBlink(char count) {
    char i;
    for(i = 0; i < count; i++) {
        setEyePower(0xff);
        _delay_ms(100);
        setEyePower(0x00);
        _delay_ms(250);
    }
}

void eyesInit() {
    ioOutput(B2);
    timer1Init(5, 2, PWM_DISCONNECTED, PWM_NONINVERTED, 0, 0, 0);    
}

void setEyePower(unsigned char power) {
    ioPower(B2, power);
}
