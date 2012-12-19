#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000
#define BAUD 9600
#include <util/setbaud.h>

char ReceivedChar;

int main (void){
    DDRD = 0b00000010;
    DDRB = 0b00000010;
    PORTB = 0b00000000;
    
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

    #if USE_2X
    UCSR0A |= (1 << U2X0);
    #else
    UCSR0A &= ~(1 << U2X0);
    #endif

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0B = (1 << RXCIE0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    sei();

    while(1) {
        
    }
   
    return(0);
}

ISR (USART_RX_vect) {
    PORTB = 0b00000010;
    ReceivedChar = UDR0; 
    UDR0 = ReceivedChar;
}
