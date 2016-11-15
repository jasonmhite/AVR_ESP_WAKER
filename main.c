#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "config.h"

/* The general flow is to first set up everything and then sleep.
 * When we are woken from sleep, send a single pulse and then
 * go back to sleep. Note that I'm using a pin change interrupt -
 * the ATTiny supports level interrupts (e.g. rising or falling edge)
 * but you have to leave the ADC powered on during sleep. Instead
 * I just use a pin change interrupt, which will trigger on both rising
 * and falling edges to wake and only send a pulse if the level
 * of the input pin is LOW. This is a bit more efficient power wise
 * in the long run.
 */

// Some helper macros to set or unset a register
// Clear bit
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
// Set bit
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void pulse() {
    // Send a single low pulse on OUTPUT_PIN
    cbi(PORTB, OUTPUT_PIN);
    _delay_ms(PULSE_DURATION);
    sbi(PORTB, OUTPUT_PIN);
}

void sleep() {
    // Put the ATTiny into deep sleep, wake on pin change interrupt
    
    cli(); // Disable interrupts while we set up sleep

        sbi(GIMSK, PCIE); // Enable pin change interrupt
        sbi(PCMSK, INPUT_PIN); // Set PCI on INPUT_PIN
        cbi(ADCSRA, ADEN); // Disable the ADC (saves power)
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    sei(); // Renable interrupts

    sleep_cpu(); // Go to sleep - when an interrupt is triggered it 
                 // will resume from the next line!

    // We have been woken from sleep
    cli();

        cbi(PCMSK, INPUT_PIN); // Disable the PCI while we are awake
        sleep_disable();
        sbi(ADCSRA, ADEN);

    sei();
}


// This is an AVR macro that sets up an interrupt service routine
ISR(PCINT0_vect) {
    // Read state of the input pin, true -> LOW
    _Bool state = bit_is_clear(PINB, INPUT_PIN);

    if (state) {
        pulse();
    }

    _delay_ms(DEBOUNCE_TIME);
}

int main() {
    // Set OUTPUT_PIN to output other pins are set to input
    DDRB = _BV(OUTPUT_PIN);

    // Set OUTPUT_PIN to high and enable internal pullups on INPUT_PIN
    PORTB = _BV(OUTPUT_PIN) | _BV(INPUT_PIN);

    while(1) { sleep(); } // Wait until trigger

    return 0;
}
