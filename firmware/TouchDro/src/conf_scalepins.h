#ifndef SCALE_PINS
#define SCALE_PINS

// I/O ports config (change pin numbers if DRO, Tach sensor or Tach LED feedback is connected to different ports)
#define SCALE_CLK_PIN            2

#define SCALE_X_PIN              3
#define SCALE_Y_PIN              4
#define SCALE_Z_PIN              5
#define SCALE_W_PIN              6


// Define registers and pins for scale ports
#if SCALE_CLK_PIN < 8
#define CLK_PIN_BIT           SCALE_CLK_PIN
#define SCALE_CLK_DDR         DDRD
#define SCALE_CLK_OUTPUT_PORT PORTD
#else
#define CLK_PIN_BIT           (SCALE_CLK_PIN - 8)
#define SCALE_CLK_DDR         DDRB
#define SCALE_CLK_OUTPUT_PORT PORTB
#endif

#if SCALE_X_PIN < 8
#define X_PIN_BIT     SCALE_X_PIN
#define X_DDR         DDRD
#define X_INPUT_PORT  PIND
#define X_OUTPUT_PORT PORTD
#else
#define X_PIN_BIT     (SCALE_X_PIN - 8)
#define X_DDR         DDRB
#define X_INPUT_PORT  PINB
#define X_OUTPUT_PORT PORTB
#endif

#if SCALE_Y_PIN < 8
#define Y_PIN_BIT     SCALE_Y_PIN
#define Y_DDR         DDRD
#define Y_INPUT_PORT  PIND
#define Y_OUTPUT_PORT PORTD
#else
#define Y_PIN_BIT     (SCALE_Y_PIN - 8)
#define Y_DDR         DDRB
#define Y_INPUT_PORT  PINB
#define Y_OUTPUT_PORT PORTB
#endif

#if SCALE_Z_PIN < 8
#define Z_PIN_BIT     SCALE_Z_PIN
#define Z_DDR         DDRD
#define Z_INPUT_PORT  PIND
#define Z_OUTPUT_PORT PORTD
#else
#define Z_PIN_BIT     (SCALE_Z_PIN - 8)
#define Z_DDR         DDRB
#define Z_INPUT_PORT  PINB
#define Z_OUTPUT_PORT PORTB
#endif

#if SCALE_W_PIN < 8
#define W_PIN_BIT     SCALE_W_PIN
#define W_DDR         DDRD
#define W_INPUT_PORT  PIND
#define W_OUTPUT_PORT PORTD
#else
#define W_PIN_BIT     (SCALE_W_PIN - 8)
#define W_DDR         DDRB
#define W_INPUT_PORT  PINB
#define W_OUTPUT_PORT PORTB
#endif

#endif