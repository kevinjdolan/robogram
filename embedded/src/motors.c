#include "settings.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "bluetooth.h"
#include "eyes.h"
#include "io.h"
#include "motors.h"
#include "timers.h"

#define GAIN_INITIAL 133
#define GAIN_PROPORTIONAL 27
#define MAX_POWER 25600
#define MIN_POWER 1280
#define WHEEL_TIMER_PRESCALER_CLOCKS 256
#define NORMALIZED_FCPU F_CPU/WHEEL_TIMER_PRESCALER_CLOCKS/WHEEL_ENCODER_CLICKS_PER_REVOLUTION

volatile unsigned int timerHigh = 0;

WheelState leftWheelState = {LEFT_ENCODER, LEFT_POWER, LEFT_DIRECTION};
WheelState rightWheelState = {RIGHT_ENCODER, RIGHT_POWER, RIGHT_DIRECTION};

unsigned int getWheelCount(char wheel) {
    return _getWheelState(wheel)->count;
}

void motorsInit() {
    ioOutput(LEFT_POWER);
    ioOutput(RIGHT_POWER);
    ioOutput(LEFT_DIRECTION);
    ioOutput(RIGHT_DIRECTION);
    timer0Init(1, 3, PWM_NONINVERTED, PWM_NONINVERTED, 0, 0, 0);
    
    ioInterrupt(LEFT_ENCODER);
    ioInterrupt(RIGHT_ENCODER);
    timer2Init(0, 6, PWM_DISCONNECTED, PWM_DISCONNECTED, 0, 0, 1);
}


void setWheelSpeed(char wheel, short speed) {
    _setWheelSpeed(_getWheelState(wheel), speed);
}

void setWheelSpeeds(short leftSpeed, short rightSpeed) {
    _setWheelSpeed(&leftWheelState, leftSpeed);
    _setWheelSpeed(&rightWheelState, rightSpeed);
}

unsigned short _getSampledRpm(WheelState *state) {
    unsigned short total = 0;
    char optimistic = 1;
    char i;
    for(i = 0; i < WHEEL_ENCODER_SAMPLE_COUNT; i++) {
        if(state->samples[i] > 0) {
            total += state->samples[i];
        }
        else {
            total += state->targetRpm;
            optimistic = 0;
        }
    }
    return total / WHEEL_ENCODER_SAMPLE_COUNT;
}

WheelState *_getWheelState(char wheel) {
    if(wheel == LEFT_WHEEL) {
        return &leftWheelState;
    }
    else {
        return &rightWheelState;
    }
}


void _readWheelState(WheelState *state) {
    if(ioReadChange(state->encoderPin) == IO_RISE) {
        if(state->risen) {
            unsigned int time = timerHigh + TCNT2;
            unsigned int dt = time - state->time;
            state->samples[state->next] = 60 * NORMALIZED_FCPU / dt;
            state->next += 1;
            if(state->next >= WHEEL_ENCODER_SAMPLE_COUNT) {
                state->next = 0;
            }
            state->time = time;
            state->count += 1;
            _updatePid(state, dt);
        }
        state->risen = 1;
    }
}

void _resetWheelState(WheelState *state, unsigned short targetRpm, char direction) {
    ioWrite(state->directionPin, direction);
    state->state = ioRead(state->encoderPin);
    state->risen = 0;
    state->time = timerHigh + TCNT2;
    state->next = 0;
    state->previousError = 0;
    state->errorIntegral = 0;
    state->count = 0;
    char i;
    for(i = 0; i < WHEEL_ENCODER_SAMPLE_COUNT; i++) {
        state->samples[i] = 0;
    }

    state->targetRpm = targetRpm;
    state->power = targetRpm * GAIN_INITIAL;
    _updatePower(state);
}

void _setWheelSpeed(WheelState *state, short speed) {
    if(speed >= 0) {
        _resetWheelState(state, speed, FORWARD);
    }
    else {
        _resetWheelState(state, -speed, REVERSE);
    }  
}

void _updatePower(WheelState *state) {
    ioPower(state->powerPin, state->power >> 7);
}

void _updatePid(WheelState *state, unsigned int dt) {
    unsigned short rpm = _getSampledRpm(state);
    if(rpm) {
        short error = state->targetRpm - rpm;
        state->power += error * GAIN_PROPORTIONAL;
        if(state->power > MAX_POWER) {
            state->power = MAX_POWER;
        }
        else if(state->power < MIN_POWER) {
            state->power = MIN_POWER;
        }
        _updatePower(state);
    }
}

ISR(TIMER2_OVF_vect) {
    timerHigh += 0x100;
}


ISR(PCINT1_vect) {
    _readWheelState(&leftWheelState);
    _readWheelState(&rightWheelState);
}

