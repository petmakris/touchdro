#ifndef CONF_PROBEPINS_H
#define CONF_PROBEPINS_H

// Touch probe config (if Touch Probe is not connected change in the corresponding constant value from "1" to "0")
#define PROBE_ENABLED   1
#define INPUT_PROBE_PIN 8  // Pin 8 connected to Touch Probe

// Define registers and pins for touch probe
#if INPUT_PROBE_PIN < 8
#define PROBE_PIN_BIT    INPUT_PROBE_PIN
#define PROBE_DDR        DDRD
#define PROBE_INPUT_PORT PIND
#else
#define PROBE_PIN_BIT    (INPUT_PROBE_PIN - 8)
#define PROBE_DDR        DDRB
#define PROBE_INPUT_PORT PINB
#endif

#if OUTPUT_PROBE_LED_PIN < 8
#define PROBE_LED_PIN_BIT     OUTPUT_PROBE_LED_PIN
#define PROBE_LED_DDR         DDRD
#define PROBE_LED_OUTPUT_PORT PORTD
#else
#define PROBE_LED_PIN_BIT     (OUTPUT_PROBE_LED_PIN - 8)
#define PROBE_LED_DDR         DDRB
#define PROBE_LED_OUTPUT_PORT PORTB
#endif

#endif