#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// hardware
#define F_CPU 14745600UL
#define UART_BAUD_RATE 9600

// settings
#define BUFFER_SIZE 32

#include <util/delay.h>

#include "uart.h"

unsigned int smallBuffer;
char numBuffer[16];
char inputTestState = 0;
volatile unsigned int irValue = 0;
char leftState = 0;
volatile unsigned int leftCount = 0;
char rightState = 0;
volatile unsigned int rightCount = 0;
unsigned int successes = 0;

void pass();
void fail();

char read();

void prepare(char txt[]);
char prompt(char question[]);
char promptWithRetry();

char inputTestContinue();
void inputTestInit(char instructions[]);
void inputTestSuccess();

void writeln(char txt[]);

int main (void) {
    DDRB |= (1 << PORTB2); // eyes
    DDRB |= (1 << PORTB0); // left motor direction
    DDRD |= (1 << PORTD7); // right motor direction
    DDRD |= (1 << PORTD6); // left motor power
    DDRD |= (1 << PORTD5); // right motor power

    PORTB |= (1 << PORTB1); // front bumper
    PORTD |= (1 << PORTD3); // left bumper
    PORTD |= (1 << PORTD2); // right bumper

    //uart setup
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU)); // bluetooth

    //timer 0 setup (motor pwm)
    OCR0A = 0; // left power
    OCR0B = 0; // right power
    TCCR0A |= (1 << COM0A1); // 10 = non-inverted mode for left
    TCCR0A &= ~(1 << COM0A0);
    TCCR0A |= (1 << COM0B1); // 10 = non-inverted mode for right
    TCCR0A &= ~(1 << COM0B0);
    TCCR0B &= ~(1 << WGM02); // 001 = phase-corrected PWM with FF top
    TCCR0A &= ~(1 << WGM01); 
    TCCR0A |= (1 << WGM00);
    TCCR0B &= ~(1 << CS02); // 011 = prescale clock/64
    TCCR0B |= (1 << CS01);
    TCCR0B |= (1 << CS00);
    
    //ADC setup
    ADMUX &= ~(1 << REFS1); // 01 = AVcc with capacitor on AREF
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << MUX3); // 0010 = USE ADC2
    ADMUX &= ~(1 << MUX2);
    ADMUX |= (1 << MUX1);
    ADMUX &= ~(1 << MUX0);
    ADMUX &= ~(1 << ADLAR); // 10-bit resolution
    ADCSRA |= (1 << ADPS2); // 111 = 128 prescale for 16Mhz
    ADCSRA | (1 << ADPS1);
    ADCSRA | (1 << ADPS0); 
    ADCSRA |= (1 << ADATE);   // Set ADC Auto Trigger Enable
    ADCSRB = 0; // 0 for free running mode
    ADCSRA |= (1 << ADEN); // Enable the ADC
    ADCSRA |= (1 << ADIE); // Enable Interrupts 
    ADCSRA |= (1 << ADSC); // Start the ADC conversion
    
    // wheel encoder interrupt setup
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT13);
    PCMSK1 |= (1 << PCINT9);
    
    sei();

    writeln("====== BEGIN ROBOGRAM TESTS ======");
    prompt("Can you read this?");
    
    prepare("Eyes will blink.");
    do {
        PORTB |= (1 << PORTB2);
        _delay_ms(500);
        PORTB &= ~(1 << PORTB2);
    } while(promptWithRetry());
    
    inputTestInit("Do not press front bumper.");
    while(inputTestContinue()) {
        if(PINB & (1 << PORTB1)) {
            inputTestSuccess();
        }
    }
    
    inputTestInit("Press front bumper.");
    while(inputTestContinue()) {
        if(!(PINB & (1 << PORTB1))) {
            inputTestSuccess();
        }
    }
     
    inputTestInit("Do not press left bumper.");
    while(inputTestContinue()) {
        if(PIND & (1 << PORTD3)) {
            inputTestSuccess();
        }
    }
    
    inputTestInit("Press left bumper.");
    while(inputTestContinue()) {
        if(!(PIND & (1 << PORTD3))) {
            inputTestSuccess();
        }
    }

    inputTestInit("Do not press right bumper.");
    while(inputTestContinue()) {
        if(PIND & (1 << PORTD2)) {
            inputTestSuccess();
        }
    }
    
    inputTestInit("Press right bumper.");
    while(inputTestContinue()) {
        if(!(PIND & (1 << PORTD2))) {
            inputTestSuccess();
        }
    }
    
    prepare("Will move forward.");
    do {
        PORTB |= (1 << PORTB0); // left dir
        PORTD |= (1 << PORTD7); //right dir
        OCR0A = 64; //left power
        OCR0B = 64; //right power
        _delay_ms(500);
        OCR0A = 0;
        OCR0B = 0;
    } while(promptWithRetry());
    
    prepare("Will move backward.");
    do {
        PORTB &= ~(1 << PORTB0);
        PORTD &= ~(1 << PORTD7);
        OCR0A = 64;
        OCR0B = 64;
        _delay_ms(500);
        OCR0A = 0;
        OCR0B = 0;
    } while(promptWithRetry());
    
    prepare("Will turn right.");
    do {
        PORTB |= (1 << PORTB0);
        OCR0A = 64;
        _delay_ms(500);
        OCR0A = 0;
    } while(promptWithRetry());
    
    prepare("Will turn left.");
    do {
        PORTD |= (1 << PORTD7);
        OCR0B = 64;
        _delay_ms(500);
        OCR0B = 0;
    } while(promptWithRetry());
    
    writeln("Autocheck left wheel encoder (will turn right)");
    rightCount = 0;
    leftCount = 0;
    PORTB |= (1 << PORTB0);
    OCR0A = 64;
    _delay_ms(500);
    OCR0A = 0;
    if(leftCount > 0 && leftCount > rightCount) {
        pass();
    }
    else {
        fail();
    }
    
    writeln("Autocheck right wheel encoder (will turn left)");
    rightCount = 0;
    leftCount = 0;
    PORTD |= (1 << PORTD7);
    OCR0B = 64;
    _delay_ms(500);
    OCR0B = 0;
    if(rightCount > 0 && rightCount > leftCount) {
        pass();
    }
    else {
        fail();
    }
    
    writeln("Place ir sensor on dark surface (any key to continue).");
    read();
    unsigned int dark = irValue;
    writeln("Place ir sensor on light surface (any key to continue).");
    read();
    unsigned int light = irValue;
    if(light - dark > 50) {
        pass();    
    }
    else {
        fail();
    }
    

    writeln("====== END ROBOGRAM TESTS ======");
    
    itoa(successes, numBuffer, 10);
    uart_puts("Total successes: ");
    writeln(numBuffer);
   
    while(1) {}
    return(0);
}

ISR(ADC_vect){
    irValue = ADCL;
    irValue |= ADCH << 8;
}

ISR(PCINT1_vect) {
    char newLeftState = PINC & (1 << PORTC1);
    if(newLeftState != leftState) {
        leftState = newLeftState;
        leftCount += 1;
    }
    char newRightState = PINC & (1 << PORTC5);
    if(newRightState != rightState) {
        rightState = newRightState;
        rightCount += 1;
    }
}

void fail() {
    writeln("YOU BROKE THE BUILD YOU DINGUS!");
}

char inputTestContinue() {
    if(inputTestState) {
        pass();
        return 0;
    }
    else if(uart_getc() == 's') {
        fail();
        return 0;
    }
    else {
        return 1;
    }
}

void inputTestInit(char instructions[]) {
    writeln(instructions);
    writeln("Press 's' to escape early.");
    inputTestState = 0;
}

void inputTestSuccess() {
    inputTestState = 1;
}

void pass() {
    successes += 1;
    writeln("GOOD, TEST PASSES.");
}

void prepare(char txt[]) {
   writeln(txt);
   writeln("(any to continue)");
   read(); 
}

char prompt(char question[]) {
    writeln(question);
    writeln("(y or n)");
    if(read() == 'y') {
        pass();
        return 1;
    }
    else {
        fail();
        return 0;
    }
}

char promptWithRetry() {
    if(prompt("Did it behave as expected?")) {
        return 0;
    }
    else {
        writeln("Try again? (y or n)");
        return read() == 'y';
    }
}

char read() {
    while(1) {
        smallBuffer = uart_getc();
        if(!(smallBuffer & UART_NO_DATA)) {
            return (char) smallBuffer;
        }
    }
}

void writeln(char txt[]) {
    uart_puts(txt);
    uart_puts("\n\r");
}
