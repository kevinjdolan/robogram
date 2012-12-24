#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// special codes
#define BACKSPACE 127
#define ENTER 13
#define MAX_PRINTABLE 126
#define MIN_PRINTABLE 32

// hardware
#define F_CPU 14745600UL
#define UART_BAUD_RATE 9600

// settings
#define BUFFER_SIZE 32

#include <util/delay.h>

#include "uart.h"

unsigned int smallBuffer;
char bigBuffer[BUFFER_SIZE];
char numBuffer[8];
char inputTestState = 0;
unsigned int successes = 0;

void pass();
void fail();

char read();
void readln();

void prepare(char txt[]);
char prompt(char question[]);
char promptWithRetry();

char inputTestContinue();
void inputTestInit(char instructions[]);
void inputTestSuccess();

void writeln(char txt[]);

int main (void) {
    DDRB |= (1 << PORTB1); // eyes
    PORTB |= (1 << PORTB2); // front bumper

    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU)); // bluetooth
    sei();

    writeln("====== BEGIN ROBOGRAM TESTS ======");
    
    prompt("Can you read this?");

    prepare("Eyes will blink.");
    do {
        PORTB |= (1 << PORTB1);
        _delay_ms(500);
        PORTB &= ~(1 << PORTB1);
    } while(promptWithRetry());
    
    inputTestInit("Do not press front bumper.");
    while(inputTestContinue()) {
        if(PINB & (PORTB2 << 1)) {
            inputTestSuccess();
        }
    }
    
    inputTestInit("Press front bumper.");
    while(inputTestContinue()) {
        if(!(PINB & (PORTB2 << 1))) {
            inputTestSuccess();
        }
    }

    writeln("====== END ROBOGRAM TESTS ======");
    
    itoa(successes, numBuffer, 10);
    uart_puts("Total successes: ");
    writeln(numBuffer);

    while(1) {}
    return(0);
}

void fail() {
    writeln("YOU BROKE THE BUILD YOU DINGUS!");
}

char inputTestContinue() {
    if(inputTestState) {
        pass();
        return 0;
    }
    else if(uart_getc() == ENTER) {
        fail();
        return 0;
    }
    else {
        return 1;
    }
}

void inputTestInit(char instructions[]) {
    writeln(instructions);
    writeln("Press enter to escape early.");
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
   writeln("(enter to continue)");
   readln(); 
}

char prompt(char question[]) {
    writeln(question);
    writeln("(yes or no)");
    readln();
    if(strcmp(bigBuffer, "yes") == 0) {
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
        writeln("Try again? (yes or no)");
        readln();
        return strcmp(bigBuffer, "yes") == 0;
    }
}

char read() {
    while(1) {
        smallBuffer = uart_getc();
        if(!(smallBuffer & UART_NO_DATA)) {
            // TODO: should check errors?
            if(smallBuffer >= MIN_PRINTABLE && smallBuffer <= MAX_PRINTABLE) {
                uart_putc((char) smallBuffer);
                return (char) smallBuffer;
            }
            else if(smallBuffer == BACKSPACE) {
                uart_puts("\b \b");    
            }
            else if(smallBuffer == ENTER) {
                uart_puts("\n\r");
            }
            return (char) smallBuffer;
        }
    }
}

void readln() {
    char next;
    char position = 0;
    while((next = read()) != ENTER) {
        if(next == BACKSPACE) {
            if(position > 0) {
                position -= 1;
            }
        }
        else {
            bigBuffer[position] = next;
            position += 1;
        }

        if (position == BUFFER_SIZE - 2) {
            writeln("");
            break;
        }
    }
    
    bigBuffer[position] = 0;
}

void writeln(char txt[]) {
    uart_puts(txt);
    uart_puts("\n\r");
}
