#include <avr/io.h>

#include "timers.h"

void timer0Init(char mode, char prescaler, char outModePD5, char outModePD6, char enableInterruptA, char enableInterruptB, char enableInterruptOverflow) {
    TCCR0A = 0;
    TCCR0B = 0;
    TIMSK0 = 0;

    TCCR0B |= ((mode >> 2) & (0b1)) << WGM02;
    TCCR0A |= (mode & 0b11) << WGM00;
    
    TCCR0B |= (prescaler << CS00);
    
    TIMSK0 |= (enableInterruptA << OCIE0A);
    TIMSK0 |= (enableInterruptB << OCIE0B);
    TIMSK0 |= (enableInterruptOverflow << TOIE0);

    TCCR0A |= (outModePD5 << COM0B0);
    TCCR0A |= (outModePD6 << COM0A0);
}

void timer1Init(char mode, char prescaler, char outModePB1, char outModePB2, char enableInterruptA, char enableInterruptB, char enableInterruptOverflow) {
    TCCR1A = 0;
    TCCR1B = 0;
    TIMSK1 = 0;

    TCCR1B |= ((mode >> 2) & (0b11)) << WGM12;
    TCCR1A |= (mode & 0b11) << WGM10;
    
    TCCR1B |= (prescaler << CS10);
    
    TIMSK1 |= (enableInterruptA << OCIE1A);
    TIMSK1 |= (enableInterruptB << OCIE1B);
    TIMSK1 |= (enableInterruptOverflow << TOIE1);

    TCCR1A |= (outModePB1 << COM1A0);
    TCCR1A |= (outModePB2 << COM1B0);
}

void timer2Init(char mode, char prescaler, char outModePB3, char outModePD3, char enableInterruptA, char enableInterruptB, char enableInterruptOverflow) {
    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 = 0;

    TCCR2B |= ((mode >> 2) & (0b1)) << WGM22;
    TCCR2A |= (mode & 0b11) << WGM20;
    
    TCCR2B |= (prescaler << CS20);
    
    TIMSK2 |= (enableInterruptA << OCIE2A);
    TIMSK2 |= (enableInterruptB << OCIE2B);
    TIMSK2 |= (enableInterruptOverflow << TOIE2);

    TCCR2A |= (outModePB3 << COM2A0);
    TCCR2A |= (outModePD3 << COM2B0);
}
