#include <avr/interrupt.h>
#include <avr/io.h>

#include "io.h"
#include "sensors.h"

volatile unsigned short irValue = 0;

void sensorsInit() {
    ioWriteHigh(FRONT_BUMPER);
    ioWriteHigh(LEFT_BUMPER);
    ioWriteHigh(RIGHT_BUMPER);
    
    // TODO: one day this may be useful abstracted
    ADMUX = 0;
    ADMUX |= 0b01 << REFS0; // AVcc with capacitor on AREF
    ADMUX |= 0b0010 << MUX0; // USE ADC2
    ADMUX |= 0b0 << ADLAR; // 10-bit resolution

    ADCSRA = 0;
    ADCSRA |= 0b111 << ADPS0; // 128 prescale for 16Mhz
    ADCSRA |= 0b1 << ADATE;   // Set ADC Auto Trigger Enable
    ADCSRA |= 0b1 << ADEN; // Enable the ADC
    ADCSRA |= 0b1 << ADIE; // Enable Interrupts 
    ADCSRA |= 0b1 << ADSC; // Start the ADC conversion
    
    ADCSRB = 0;
}

unsigned short getIrValue() {
    return irValue;
}

ISR(ADC_vect){
    irValue = ADCL;
    irValue |= ADCH << 8;
}
