#ifndef CONF_TACHPINS_H
#define CONF_TACHPINS_H

// Tach config (if Tach is not connected change in the corresponding constant value from "1" to "0")
#define TACH_ENABLED             0
#define INPUT_TACH_PIN           7


// Define tach interrupt for selected pin
#if INPUT_TACH_PIN == 2
#define TACH_PIN_BIT            2
#define TACH_DDR                DDRD
#define TACH_INPUT_PORT         PIND
#define TACH_INTERRUPT_VECTOR   PCINT2_vect
#define TACH_INTERRUPT_REGISTER PCIE2
#define TACH_INTERRUPT_MASK     PCMSK2
#define TACH_INTERRUPT_PIN      PCINT18

#elif INPUT_TACH_PIN == 3
#define TACH_PIN_BIT            3
#define TACH_DDR                DDRD
#define TACH_INPUT_PORT         PIND
#define TACH_INTERRUPT_VECTOR   PCINT2_vect
#define TACH_INTERRUPT_REGISTER PCIE2
#define TACH_INTERRUPT_MASK     PCMSK2
#define TACH_INTERRUPT_PIN      PCINT19

#elif INPUT_TACH_PIN == 4
#define TACH_PIN_BIT            4
#define TACH_DDR                DDRD
#define TACH_INPUT_PORT         PIND
#define TACH_INTERRUPT_VECTOR   PCINT2_vect
#define TACH_INTERRUPT_REGISTER PCIE2
#define TACH_INTERRUPT_MASK     PCMSK2
#define TACH_INTERRUPT_PIN      PCINT20

#elif INPUT_TACH_PIN == 5
#define TACH_PIN_BIT            5
#define TACH_DDR                DDRD
#define TACH_INPUT_PORT         PIND
#define TACH_INTERRUPT_VECTOR   PCINT2_vect
#define TACH_INTERRUPT_REGISTER PCIE2
#define TACH_INTERRUPT_MASK     PCMSK2
#define TACH_INTERRUPT_PIN      PCINT21

#elif INPUT_TACH_PIN == 6
#define TACH_PIN_BIT            6
#define TACH_DDR                DDRD
#define TACH_INPUT_PORT         PIND
#define TACH_INTERRUPT_VECTOR   PCINT2_vect
#define TACH_INTERRUPT_REGISTER PCIE2
#define TACH_INTERRUPT_MASK     PCMSK2
#define TACH_INTERRUPT_PIN      PCINT22

#elif INPUT_TACH_PIN == 7
#define TACH_PIN_BIT            7
#define TACH_DDR                DDRD
#define TACH_INPUT_PORT         PIND
#define TACH_INTERRUPT_VECTOR   PCINT2_vect
#define TACH_INTERRUPT_REGISTER PCIE2
#define TACH_INTERRUPT_MASK     PCMSK2
#define TACH_INTERRUPT_PIN      PCINT23

#elif INPUT_TACH_PIN == 8
#define TACH_PIN_BIT            0
#define TACH_DDR                DDRB
#define TACH_INPUT_PORT         PINB
#define TACH_INTERRUPT_VECTOR   PCINT0_vect
#define TACH_INTERRUPT_REGISTER PCIE0
#define TACH_INTERRUPT_MASK     PCMSK0
#define TACH_INTERRUPT_PIN      PCINT0

#elif INPUT_TACH_PIN == 9
#define TACH_PIN_BIT            1
#define TACH_DDR                DDRB
#define TACH_INPUT_PORT         PINB
#define TACH_INTERRUPT_VECTOR   PCINT0_vect
#define TACH_INTERRUPT_REGISTER PCIE0
#define TACH_INTERRUPT_MASK     PCMSK0
#define TACH_INTERRUPT_PIN      PCINT1

#elif INPUT_TACH_PIN == 10
#define TACH_PIN_BIT            2
#define TACH_DDR                DDRB
#define TACH_INPUT_PORT         PINB
#define TACH_INTERRUPT_VECTOR   PCINT0_vect
#define TACH_INTERRUPT_REGISTER PCIE0
#define TACH_INTERRUPT_MASK     PCMSK0
#define TACH_INTERRUPT_PIN      PCINT2

#elif INPUT_TACH_PIN == 11
#define TACH_PIN_BIT            3
#define TACH_DDR                DDRB
#define TACH_INPUT_PORT         PINB
#define TACH_INTERRUPT_VECTOR   PCINT0_vect
#define TACH_INTERRUPT_REGISTER PCIE0
#define TACH_INTERRUPT_MASK     PCMSK0
#define TACH_INTERRUPT_PIN      PCINT3

#elif INPUT_TACH_PIN == 12
#define TACH_PIN_BIT            4
#define TACH_DDR                DDRB
#define TACH_INPUT_PORT         PINB
#define TACH_INTERRUPT_VECTOR   PCINT0_vect
#define TACH_INTERRUPT_REGISTER PCIE0
#define TACH_INTERRUPT_MASK     PCMSK0
#define TACH_INTERRUPT_PIN      PCINT4

#elif INPUT_TACH_PIN == 13
#define TACH_PIN_BIT            5
#define TACH_DDR                DDRB
#define TACH_INPUT_PORT         PINB
#define TACH_INTERRUPT_VECTOR   PCINT0_vect
#define TACH_INTERRUPT_REGISTER PCIE0
#define TACH_INTERRUPT_MASK     PCMSK0
#define TACH_INTERRUPT_PIN      PCINT5
#endif

#if OUTPUT_TACH_LED_PIN < 8
#define TACH_LED_PIN_BIT     OUTPUT_TACH_LED_PIN
#define TACH_LED_DDR         DDRD
#define TACH_LED_OUTPUT_PORT PORTD
#else
#define TACH_LED_PIN_BIT     (OUTPUT_TACH_LED_PIN - 8)
#define TACH_LED_DDR         DDRB
#define TACH_LED_OUTPUT_PORT PORTB
#endif


#endif 