#ifndef __CONFIG_H__
#define __CONFIG_H__ 1

#define INPUT_PIN PCINT3
#define OUTPUT_PIN PB0

// Length of pulse to send to the ESP RST
// 30 ms is enough to trigger a reset
#define PULSE_DURATION 30

// Debounce time on the edge trigger
// adjust if unstable
#define DEBOUNCE_TIME 100

#endif
