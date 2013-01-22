#include "settings.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#include "bluetooth.h"
#include "eyes.h"
#include "io.h"
#include "motors.h"
#include "timers.h"
#include "sensors.h"

int main (void) {
    bluetoothInit();
    eyesInit();
    motorsInit();
    sensorsInit();
    ioChangeInit();
    
    sei();

    writeln("ready?");
    read();
    while(1) {
        writeNumber(getIrValue());
        writeln("");
        _delay_ms(100);
    }
    return(0);
}
