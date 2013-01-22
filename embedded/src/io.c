#include "settings.h"

#include <avr/io.h>
#include <util/delay.h>

#include "io.h"

#define MASK_C 0x8
#define MASK_D 0x10
#define MASK_PIN 0x7

unsigned char portBMemory;
unsigned char portCMemory;
unsigned char portDMemory;

void ioChangeInit() {
    portBMemory = PINB;
    portCMemory = PINC;
    portDMemory = PIND;
}

void ioInterrupt(char pin) {
    char mask = 1 << (pin & MASK_PIN);
    if(pin & MASK_D) {
        PCMSK2 |= mask;
        PCICR |= (1 << PCIE2);
    }
    else if(pin & MASK_C) {
        PCMSK1 |= mask;
        PCICR |= (1 << PCIE1);
    }
    else {
        PCMSK0 |= mask;
        PCICR |= (1 << PCIE0);
    }
}

void ioOutput(char pin) {
    char mask = 1 << (pin & MASK_PIN);
    if(pin & MASK_D) {
        DDRD |= mask;
    }
    else if(pin & MASK_C) {
        DDRC |= mask;
    }
    else {
        DDRB |= mask;
    }
}

void ioPower(char pin, unsigned short power) {
    if(pin == B1) {
        OCR1A = power;
    }
    else if(pin == B2) {
        OCR1B = power;
    }
    else if(pin == B3) {
        OCR2A = power;
    }
    else if(pin == D3) {
        OCR2B = power;
    }
    else if(pin == D5) {
        OCR0B = power;
    }
    else if(pin == D6) {
        OCR0A = power;
    }
}

char ioRead(char pin) {
    char mask = 1 << (pin & MASK_PIN);
    if(pin & MASK_D) {
        return PIND & mask;
    }
    else if(pin & MASK_C) {
        return PINC & mask;
    }
    else {
        return PINB & mask;
    }
}

char ioReadChange(char pin) {
    char mask = 1 << (pin & MASK_PIN);

    char previousValue;
    if(pin & MASK_D) {
        previousValue = portDMemory & mask;
    }
    else if(pin & MASK_C) {
        previousValue = portCMemory & mask;
    }
    else {
        previousValue = portBMemory & mask;
    }
    
    char newValue = ioRead(pin);
    if(newValue != previousValue) {
        // looks like a change, better double check
        _delay_us(DEBOUNCE_TIME);
        newValue = ioRead(pin);
    }
    
    if(previousValue != newValue) {
        if(pin & MASK_D) {
            portDMemory ^= mask;
        }
        else if(pin & MASK_C) {
            portCMemory ^= mask;
        }
        else {
            portBMemory ^= mask;
        }
        
        if(newValue) {
            return IO_RISE;
        }
        else {
            return IO_FALL;
        }
    }
    else {
        if(newValue) {
            return IO_HIGH;
        }
        else {
            return IO_LOW;
        }
    }
}

void ioWrite(char pin, char value) {
    if(value) {
        ioWriteHigh(pin);
    }
    else {
        ioWriteLow(pin);
    }
}

void ioWriteHigh(char pin) {
    char mask = 1 << (pin & MASK_PIN);
    if(pin & MASK_D) {
        PORTD |= mask;
    }
    else if(pin & MASK_C) {
        PORTC |= mask;
    }
    else {
        PORTB |= mask;
    }
}

void ioWriteLow(char pin) {
    char mask = 1 << (pin & MASK_PIN);
    if(pin & MASK_D) {
        PORTD &= ~mask;    
    }
    else if(pin & MASK_C) {
        PORTC &= ~mask;    
    }
    else {
        PORTB &= ~mask;    
    }
}
