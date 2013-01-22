#import "io.h"
#import "timers.h"

void eyesInit() {
    ioOutput(B2);
    timer1Init(5, 2, PWM_DISCONNECTED, PWM_NONINVERTED, 0, 0, 0);    
}

void setEyePower(unsigned char power) {
    ioPower(B2, power);
}
