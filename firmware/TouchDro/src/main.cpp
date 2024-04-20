#include <Arduino.h>

// DRO config (if axis is not connected change in the corresponding constant value from "1" to "0")
#define SCALE_X_ENABLED         0
#define SCALE_Y_ENABLED         0
#define SCALE_Z_ENABLED         0
#define SCALE_W_ENABLED         0

// DRO config (if axis is connected to Quadrature Encoder scales through LS7366R-type shield change in the corresponding constant value from "0" to "1")
#define SCALE_X_TYPE            0
#define SCALE_Y_TYPE            0
#define SCALE_Z_TYPE            0
#define SCALE_W_TYPE            0

// I/O ports config (change pin numbers if DRO, Tach sensor or Tach LED feedback is connected to different ports)
#define SCALE_CLK_PIN           2

#define SCALE_X_PIN             3
#define SCALE_Y_PIN             4
#define SCALE_Z_PIN             5
#define SCALE_W_PIN             6

#define SCALE_X_AVERAGE_ENABLED 1
#define SCALE_Y_AVERAGE_ENABLED 1
#define SCALE_Z_AVERAGE_ENABLED 1
#define SCALE_W_AVERAGE_ENABLED 1

// Change it to 16 for machines with power feed
#define AXIS_AVERAGE_COUNT      24

// Tach config (if Tach is not connected change in the corresponding constant value from "1" to "0")
#define TACH_ENABLED            0
#define INPUT_TACH_PIN          7

// Tach pre-scale value (number of tach sensor pulses per revolution)
#define TACH_PRESCALE           1

// Number of tach measurements to average
#define TACH_AVERAGE_COUNT      6

// This is rounding for tachometer display (set to 0 to disable or 1 for 1% rounding)
#define TACH_ROUND              2

// Tach data format
#define TACH_RAW_DATA_FORMAT    0  // single value format: T<rpm>;

// Tach RPM config
#define MIN_RPM_DELAY           1200  // 1.2 sec calculates to low range = 50 rpm.

// Tach LED feadback config
#define OUTPUT_TACH_LED_ENABLED 1
#define OUTPUT_TACH_LED_PIN     9

// Touch probe config (if Touch Probe is not connected change in the corresponding constant value from "1" to "0")
#define PROBE_ENABLED           1
#define INPUT_PROBE_PIN         8  // Pin 8 connected to Touch Probe

// Touch probe invert signal config
#define PROBE_INVERT \
  0  // Touch Probe signal inversion: Open = Input pin is Low; Closed = Input pin is High

// Touch probe LED feadback config
#define OUTPUT_PROBE_LED_ENABLED 1

// When Quadrature Encoder scale are not used, on Arduino Uno you may change it to on-board LED pin 13.
#define OUTPUT_PROBE_LED_PIN     10

// General Settings
//  Set this so it matches the BT module's BAUD rate
#define UART_BAUD_RATE           9600
//  Frequency in Hz (number of timer per second the scales are read and the data is sent to the application)
#define UPDATE_FREQUENCY         24

//  Max Frequency in Hz (number of timer per second) the tach output is sent to the application
#define TACH_UPDATE_FREQUENCY    4

//---END OF CONFIGURATION PARAMETERS ---

//---DO NOT CHANGE THE CODE BELOW UNLESS YOU KNOW WHAT YOU ARE DOING ---

/* iGaging Clock Settings (do not change) */
#define SCALE_CLK_PULSES         21  // iGaging and Accuremote scales use 21 bit format
#define SCALE_CLK_FREQUENCY      9000  // iGaging scales run at about 9-10KHz
#define SCALE_CLK_DUTY \
  20  // iGaging scales clock run at 20% PWM duty (22us = ON out of 111us cycle)

/* weighted average constants */
#define FILTER_SLOW_EMA AXIS_AVERAGE_COUNT  // Slow movement EMA
#define FILTER_FAST_EMA 2                   // Fast movement EMA

#if (SCALE_X_ENABLED > 0) || (SCALE_Y_ENABLED > 0) || (SCALE_Z_ENABLED > 0) || \
    (SCALE_W_ENABLED > 0)
#define DRO_ENABLED 1
#else
#define DRO_ENABLED 0
#endif

#if (SCALE_X_ENABLED > 0 && SCALE_X_TYPE == 0) || \
    (SCALE_Y_ENABLED > 0 && SCALE_Y_TYPE == 0) || \
    (SCALE_Z_ENABLED > 0 && SCALE_Z_TYPE == 0) || \
    (SCALE_W_ENABLED > 0 && SCALE_W_TYPE == 0)
#define DRO_TYPE0_ENABLED 1
#else
#define DRO_TYPE0_ENABLED 0
#endif

#if (SCALE_X_ENABLED > 0 && SCALE_X_TYPE == 1) || \
    (SCALE_Y_ENABLED > 0 && SCALE_Y_TYPE == 1) || \
    (SCALE_Z_ENABLED > 0 && SCALE_Z_TYPE == 1) || \
    (SCALE_W_ENABLED > 0 && SCALE_W_TYPE == 1)
#define DRO_TYPE1_ENABLED 1
#else
#define DRO_TYPE1_ENABLED 0
#endif

#if (SCALE_X_AVERAGE_ENABLED > 0) || (SCALE_Y_AVERAGE_ENABLED > 0) || \
    (SCALE_Z_AVERAGE_ENABLED > 0) || (SCALE_W_AVERAGE_ENABLED > 0)
#define SCALE_AVERAGE_ENABLED 1
#else
#define SCALE_AVERAGE_ENABLED 0
#endif

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

#if DRO_TYPE1_ENABLED
#include <SPI.h>
#endif
// Some constants calculated here
unsigned long const minRpmTime = (((long)MIN_RPM_DELAY) * ((long)1000));
long const          longMax    = __LONG_MAX__;
long const          longMin    = (-__LONG_MAX__ - (long)1);
long const slowSc = ((long)2000) / (((long)FILTER_SLOW_EMA) + ((long)1));
long const fastSc = ((long)20) / (((long)FILTER_FAST_EMA) + ((long)1));

#if TACH_UPDATE_FREQUENCY == UPDATE_FREQUENCY
int const tachUpdateFrequencyCounterLimit = 1;
#else
int const tachUpdateFrequencyCounterLimit =
    (((long)UPDATE_FREQUENCY) / ((long)TACH_UPDATE_FREQUENCY));
#endif

int const updateFrequencyCounterLimit =
    (int)(((unsigned long)SCALE_CLK_FREQUENCY) /
          ((unsigned long)UPDATE_FREQUENCY));
int const clockCounterLimit =
    (int)(((unsigned long)(F_CPU / 8)) / (unsigned long)SCALE_CLK_FREQUENCY) -
    10;
int const scaleClockDutyLimit =
    (int)(((unsigned long)(F_CPU / 800)) * ((unsigned long)SCALE_CLK_DUTY) /
          (unsigned long)SCALE_CLK_FREQUENCY);
int const scaleClockFirstReadDelay = (int)((unsigned long)F_CPU / 4000000);

// variables that will store tach info and status
volatile unsigned long tachInterruptTimer;
volatile unsigned long tachInterruptRotationCount;

volatile unsigned long tachTimerStart;

// variables that will store the readout output
volatile unsigned long tachReadoutRotationCount;
volatile unsigned long tachReadoutMicrosec;
volatile unsigned long tachReadoutRpm;

#if TACH_AVERAGE_COUNT > 1
volatile unsigned long tachLastRead[TACH_AVERAGE_COUNT];
volatile int           tachLastReadPosition;
#endif

volatile int  tachUpdateFrequencyCounter;
volatile bool sendTachData;

// variable to store the touch probe status.
volatile unsigned int probeReportedValue;

// variables that will store the DRO readout
volatile bool tickTimerFlag;
volatile int  updateFrequencyCounter;

// Axis count
#if SCALE_X_ENABLED > 0
volatile long xValue;
volatile long xReportedValue;
#endif
#if SCALE_X_AVERAGE_ENABLED > 0
volatile long axisLastReadX[AXIS_AVERAGE_COUNT];
volatile int  axisLastReadPositionX;
volatile long axisAMAValueX;
#endif

#if SCALE_Y_ENABLED > 0
volatile long yValue;
volatile long yReportedValue;
#endif
#if SCALE_Y_AVERAGE_ENABLED > 0
volatile long axisLastReadY[AXIS_AVERAGE_COUNT];
volatile int  axisLastReadPositionY;
volatile long axisAMAValueY;
#endif

#if SCALE_Z_ENABLED > 0
volatile long zValue;
volatile long zReportedValue;
#endif
#if SCALE_Z_AVERAGE_ENABLED > 0
volatile long axisLastReadZ[AXIS_AVERAGE_COUNT];
volatile int  axisLastReadPositionZ;
volatile long axisAMAValueZ;
#endif

#if SCALE_W_ENABLED > 0
volatile long wValue;
volatile long wReportedValue;
#endif
#if SCALE_W_AVERAGE_ENABLED > 0
volatile long axisLastReadW[AXIS_AVERAGE_COUNT];
volatile int  axisLastReadPositionW;
volatile long axisAMAValueW;
#endif

#if DRO_TYPE1_ENABLED > 0
volatile unsigned int encoderValue1;
volatile unsigned int encoderValue2;
volatile unsigned int encoderValue3;
volatile unsigned int encoderValue4;
#endif

void setupClkTimer() {
  updateFrequencyCounter = 0;

  TCCR2A                 = 0;  // set entire TCCR2A register to 0
  TCCR2B                 = 0;  // same for TCCR2B

  // set compare match registers
#if DRO_TYPE0_ENABLED > 0
  OCR2A = scaleClockDutyLimit;  // default 44 = 22us
#else
  OCR2A = clockCounterLimit - 1;
#endif
  OCR2B = clockCounterLimit;  // default 222 = 111us

  // turn on Fast PWM mode
  TCCR2A |= _BV(WGM21) | _BV(WGM20);

  // Set CS21 bit for 8 prescaler //CS20 for no prescaler
  TCCR2B |= _BV(CS21);

  // initialize counter value to start at low pulse
#if DRO_TYPE0_ENABLED > 0
  TCNT2 = scaleClockDutyLimit + 1;
#else
  TCNT2 = 0;
#endif
  // enable timer compare interrupt A and B
  TIMSK2 |= _BV(OCIE2A) | _BV(OCIE2B);
}

/* Interrupt Service Routines */

// Timer 2 interrupt B ( Switches clock pin from low to high 21 times) at the end of clock counter limit
ISR(TIMER2_COMPB_vect) {

  // Set counter back to zero
  TCNT2 = 0;
#if DRO_TYPE0_ENABLED > 0
  // Only set the clock high if updateFrequencyCounter less than 21
  if (updateFrequencyCounter < SCALE_CLK_PULSES) {
    // Set clock pin high
    SCALE_CLK_OUTPUT_PORT |= _BV(CLK_PIN_BIT);
  }
#endif
}

// Timer 2 interrupt A ( Switches clock pin from high to low) at the end of clock PWM Duty counter limit
ISR(TIMER2_COMPA_vect) {
#if DRO_TYPE0_ENABLED > 0
  // Control the scale clock for only first 21 loops
  if (updateFrequencyCounter < SCALE_CLK_PULSES) {

    // Set clock low if high and then delay 2us
    if (SCALE_CLK_OUTPUT_PORT & _BV(CLK_PIN_BIT)) {
      SCALE_CLK_OUTPUT_PORT &= ~_BV(CLK_PIN_BIT);
      TCNT2 = scaleClockDutyLimit - scaleClockFirstReadDelay;
      return;
    }

    // read the pin state and shift it into the appropriate variables
    // Logic by Les Jones:
    //	If data pin is HIGH set bit 20th of the axis value to '1'.  Then shift axis value one bit to the right
    //  This is called 20 times (for bits received from 0 to 19)
    if (updateFrequencyCounter < SCALE_CLK_PULSES - 1) {
#if SCALE_X_ENABLED > 0 && SCALE_X_TYPE == 0
      if (X_INPUT_PORT & _BV(X_PIN_BIT))
        xValue |= ((long)0x00100000);
      xValue >>= 1;
#endif

#if SCALE_Y_ENABLED > 0 && SCALE_Y_TYPE == 0
      if (Y_INPUT_PORT & _BV(Y_PIN_BIT))
        yValue |= ((long)0x00100000);
      yValue >>= 1;
#endif

#if SCALE_Z_ENABLED > 0 && SCALE_Z_TYPE == 0
      if (Z_INPUT_PORT & _BV(Z_PIN_BIT))
        zValue |= ((long)0x00100000);
      zValue >>= 1;
#endif

#if SCALE_W_ENABLED > 0 && SCALE_W_TYPE == 0
      if (W_INPUT_PORT & _BV(W_PIN_BIT))
        wValue |= ((long)0x00100000);
      wValue >>= 1;
#endif

    } else if (updateFrequencyCounter == SCALE_CLK_PULSES - 1) {

      // If 21-st bit is 'HIGH' inverse the sign of the axis readout
#if SCALE_X_ENABLED > 0 && SCALE_X_TYPE == 0
      if (X_INPUT_PORT & _BV(X_PIN_BIT))
        xValue |= ((long)0xfff00000);
      xReportedValue = xValue;
      xValue         = 0L;
#endif

#if SCALE_Y_ENABLED > 0 && SCALE_Y_TYPE == 0
      if (Y_INPUT_PORT & _BV(Y_PIN_BIT))
        yValue |= ((long)0xfff00000);
      yReportedValue = yValue;
      yValue         = 0L;
#endif

#if SCALE_Z_ENABLED > 0 && SCALE_Z_TYPE == 0
      if (Z_INPUT_PORT & _BV(Z_PIN_BIT))
        zValue |= ((long)0xfff00000);
      zReportedValue = zValue;
      zValue         = 0L;
#endif

#if SCALE_W_ENABLED > 0 && SCALE_W_TYPE == 0
      if (W_INPUT_PORT & _BV(W_PIN_BIT))
        wValue |= ((long)0xfff00000);
      wReportedValue = wValue;
      wValue         = 0L;
#endif
      // Tell the main loop, that it's time to sent data
      tickTimerFlag = true;
    }
  }
#else
  if (updateFrequencyCounter == 0) {
    // Tell the main loop, that it's time to sent data
    tickTimerFlag = true;
  }
#endif

  updateFrequencyCounter++;
  // Start of next cycle
  if (updateFrequencyCounter >= updateFrequencyCounterLimit) {
    updateFrequencyCounter = 0;
  }
}

#if DRO_TYPE1_ENABLED
inline void readEncoders() {

#if SCALE_X_ENABLED > 0 && SCALE_X_TYPE == 1
  X_OUTPUT_PORT &= ~_BV(X_PIN_BIT);
  readEncoderValue();
  X_OUTPUT_PORT |= _BV(X_PIN_BIT);
  xReportedValue = ((long)encoderValue1 << 24) + ((long)encoderValue2 << 16) +
                   ((long)encoderValue3 << 8) + (long)encoderValue4;
#endif

#if SCALE_Y_ENABLED > 0 && SCALE_Y_TYPE == 1
  Y_OUTPUT_PORT &= ~_BV(Y_PIN_BIT);
  readEncoderValue();
  Y_OUTPUT_PORT |= _BV(Y_PIN_BIT);
  yReportedValue = ((long)encoderValue1 << 24) + ((long)encoderValue2 << 16) +
                   ((long)encoderValue3 << 8) + (long)encoderValue4;
#endif

#if SCALE_Z_ENABLED > 0 && SCALE_Z_TYPE == 1
  Z_OUTPUT_PORT &= ~_BV(Z_PIN_BIT);
  readEncoderValue();
  Z_OUTPUT_PORT |= _BV(Z_PIN_BIT);
  zReportedValue = ((long)encoderValue1 << 24) + ((long)encoderValue2 << 16) +
                   ((long)encoderValue3 << 8) + (long)encoderValue4;
#endif

#if SCALE_W_ENABLED > 0 && SCALE_W_TYPE == 1
  W_OUTPUT_PORT &= ~_BV(W_PIN_BIT);
  readEncoderValue();
  W_OUTPUT_PORT |= _BV(W_PIN_BIT);
  wReportedValue = ((long)encoderValue1 << 24) + ((long)encoderValue2 << 16) +
                   ((long)encoderValue3 << 8) + (long)encoderValue4;
#endif
}

inline void readEncoderValue() {
  SPI.transfer(0x60);                  // Request count
  encoderValue1 = SPI.transfer(0x00);  // Read highest order byte
  encoderValue2 = SPI.transfer(0x00);
  encoderValue3 = SPI.transfer(0x00);
  encoderValue4 = SPI.transfer(0x00);  // Read lowest order byte
}

#endif

#if DRO_ENABLED > 0
#if SCALE_AVERAGE_ENABLED > 0
inline void initializeAxisAverage(volatile long  axisLastRead[],
                                  volatile int&  axisLastReadPosition,
                                  volatile long& axisAMAValue) {

  for (axisLastReadPosition = 0; axisLastReadPosition < (int)AXIS_AVERAGE_COUNT;
       axisLastReadPosition++) {
    axisLastRead[axisLastReadPosition] = 0;
  }
  axisLastReadPosition = 0;
  axisAMAValue         = 0;
}

inline void scaleValueRounded(volatile long& ReportedValue,
                              volatile long  axisLastRead[],
                              volatile int&  axisLastReadPosition,
                              volatile long& axisAMAValue) {

  int last_pos;
  int first_pos;
  int prev_pos;
  int filter_pos;

  long dir;
  long minValue   = longMax;
  long maxValue   = longMin;
  long volatility = 0;
  long valueRange;
  long ssc;
  long constant;
  long delta;

  // Save current read and increment position
  axisLastRead[axisLastReadPosition] = ReportedValue;
  last_pos                           = axisLastReadPosition;

  axisLastReadPosition++;
  if (axisLastReadPosition == (int)AXIS_AVERAGE_COUNT) {
    axisLastReadPosition = 0;
  }
  first_pos = axisLastReadPosition;

  dir       = (axisLastRead[first_pos] - axisLastRead[last_pos]) * ((long)100);

  // Calculate the volatility in the counts by taking the sum of the differences
  prev_pos  = first_pos;
  for (filter_pos = (first_pos + 1) % AXIS_AVERAGE_COUNT;
       filter_pos != first_pos;
       filter_pos = (filter_pos + 1) % AXIS_AVERAGE_COUNT) {
    minValue = MIN(minValue, axisLastRead[filter_pos]);
    maxValue = MAX(maxValue, axisLastRead[filter_pos]);
    volatility += ABS(axisLastRead[filter_pos] - axisLastRead[prev_pos]);
    prev_pos = filter_pos;
  }

  // Just return the read if there is no volatility to avoid divide by 0
  if (volatility == (long)0) {
    axisAMAValue = axisLastRead[last_pos] * ((long)100);
    return;
  }

  // If the last AMA is not within twice the sample range, then assume the position jumped
  // and reset the AMA to the current read
  maxValue   = maxValue * ((long)100);
  minValue   = minValue * ((long)100);
  valueRange = maxValue - minValue;
  if (axisAMAValue > maxValue + valueRange + ((long)100) ||
      axisAMAValue < minValue - valueRange - ((long)100)) {
    axisAMAValue = axisLastRead[last_pos] * ((long)100);
    return;
  }

  // Calculate the smoothing constant
  ssc           = (ABS(dir / volatility) * fastSc) + slowSc;
  constant      = (ssc * ssc) / ((long)10000);

  // Calculate the new average
  delta         = axisLastRead[last_pos] - (axisAMAValue / ((long)100));
  axisAMAValue  = axisAMAValue + constant * delta;

  ReportedValue = (axisAMAValue + ((long)50)) / ((long)100);
  return;
}

inline long MIN(long value1, long value2) {
  if (value1 > value2) {
    return value2;
  } else {
    return value1;
  }
}

inline long MAX(long value1, long value2) {
  if (value1 > value2) {
    return value1;
  } else {
    return value2;
  }
}

inline long ABS(long value) {
  if (value < 0) {
    return -value;
  } else {
    return value;
  }
}

#endif
#endif

// Calculate the tach rpm
#if TACH_ENABLED > 0
inline bool sendTachOutputData() {
  unsigned long microSeconds;
  unsigned long tachRotationCount;
  unsigned long tachTimer;
  unsigned long currentMicros;

  // Read data from the last interrupt (stop interrupts to read a pair in sync)
  cli();
  tachRotationCount          = tachInterruptRotationCount;
  tachInterruptRotationCount = 0;
  tachTimer                  = tachInterruptTimer;
  sei();

  // reset values and ignore this readout if clock or rotation counter overlapses
  if (tachTimer < tachTimerStart) {
    tachTimerStart = tachTimer;
    return false;
  }

  // We have at least one tick on rpm sensor so calculate the time between ticks
  if (tachRotationCount != 0) {
    tachReadoutRotationCount = tachRotationCount;
    tachReadoutMicrosec      = tachTimer - tachTimerStart;

    tachTimerStart           = tachTimer;

    // if no ticks on rpm sensor...
  } else {
    currentMicros = micros();
    // reset timer if clock overlapses
    if (currentMicros < tachTimerStart) {
      tachTimerStart = 0;
      return false;
    } else {
      // if no pulses for longer than minRpmTime then set rpm to zero
      microSeconds = currentMicros - tachTimerStart;
      if (microSeconds > minRpmTime) {
        tachReadoutRotationCount = 0;
        tachReadoutMicrosec      = 0;
      } else {
        return false;
      }
    }
  }

#if TACH_RAW_DATA_FORMAT == 0
  // Calculate RPM
  if (tachReadoutRotationCount == 0) {
    tachReadoutRpm = 0;
  } else {
    unsigned long averageTime = tachReadoutMicrosec / tachReadoutRotationCount;
    // Ignore when time is zero
    if (averageTime == 0) {
      return false;
    } else {
      tachReadoutRpm = ((unsigned long)600000000 / averageTime);
      tachReadoutRpm = ((unsigned long)tachReadoutRpm / TACH_PRESCALE) + 5;
      tachReadoutRpm = ((unsigned long)tachReadoutRpm / 10);
    }
  }

#if TACH_AVERAGE_COUNT > 1
  // calculate Average RPM
  unsigned long tachReadSum;
  unsigned long tachLastReadRpm;
  int           readCounter;
  int           readCounted;

  // Save last reading
  tachLastReadRpm = tachLastRead[tachLastReadPosition];

  // Increment tachLastReadPosition
  tachLastReadPosition++;
  if (tachLastReadPosition == (int)TACH_AVERAGE_COUNT) {
    tachLastReadPosition = 0;
  }
  // Save current read
  tachLastRead[tachLastReadPosition] = tachReadoutRpm;

  // At least two consecutive measurements must be valid to calculate average
  readCounted                        = 0;
  tachReadSum                        = 0;
  if (tachReadoutRpm != 0 && tachLastReadRpm != 0) {
    // Calculate average read
    for (readCounter = 0; readCounter < (int)TACH_AVERAGE_COUNT;
         readCounter++) {
      if (tachLastRead[readCounter] != 0) {
        tachReadSum = tachReadSum + tachLastRead[readCounter];
        readCounted++;
      }
    }
  }
  if (readCounted != 0) {
    tachReadoutRpm = ((unsigned long)tachReadSum / ((int)readCounted));
  } else {
    tachReadoutRpm = 0;
  }
#endif

#if TACH_ROUND > 0
  // calculate Rounded RPM
  unsigned long tachReadRoundFactor;

  // fixed threasholds rounding
#if TACH_ROUND > 1
  if (tachReadoutRpm < 200) {
    tachReadRoundFactor = 1;
  } else if (tachReadoutRpm < 500) {
    tachReadRoundFactor = 2;
  } else if (tachReadoutRpm < 2000) {
    tachReadRoundFactor = 5;
  } else if (tachReadoutRpm < 5000) {
    tachReadRoundFactor = 10;
  } else if (tachReadoutRpm < 20000) {
    tachReadRoundFactor = 20;
  } else if (tachReadoutRpm < 50000) {
    tachReadRoundFactor = 50;
  } else {
    tachReadRoundFactor = 100;
  }

  // 1% rounding
#else
  // Determine rounding factor
  tachReadRoundFactor = (unsigned long)((tachReadoutRpm * 10) / ((int)100));
  tachReadRoundFactor = ((unsigned long)tachReadRoundFactor / 10);
  if (tachReadRoundFactor == 0) {
    tachReadRoundFactor = 1;
  }
#endif

  // Round result
  tachReadoutRpm =
      ((unsigned long)((tachReadoutRpm * 10) / tachReadRoundFactor) + 5);
  tachReadoutRpm = ((unsigned long)tachReadoutRpm / 10);
  tachReadoutRpm = ((unsigned long)tachReadoutRpm * tachReadRoundFactor);
#endif

#endif

  return true;
}
#endif

// Interrupt to read tach pin change
#if TACH_ENABLED > 0
ISR(TACH_INTERRUPT_VECTOR) {
  if (TACH_INPUT_PORT & _BV(TACH_PIN_BIT)) {
    // record timestamp of change in port input
    tachInterruptTimer = micros();
    tachInterruptRotationCount++;
#if OUTPUT_TACH_LED_ENABLED > 0
    TACH_LED_OUTPUT_PORT |= _BV(TACH_LED_PIN_BIT);
#endif
  } else {
#if OUTPUT_TACH_LED_ENABLED > 0
    TACH_LED_OUTPUT_PORT &= ~_BV(TACH_LED_PIN_BIT);
#endif
  }
}
#endif

// Read touch probe status
#if PROBE_ENABLED > 0
inline unsigned int readProbeOutputData() {
  if (PROBE_INPUT_PORT & _BV(PROBE_PIN_BIT)) {
    // Return probe signal
#if PROBE_INVERT == 0
#if OUTPUT_PROBE_LED_ENABLED > 0
    PROBE_LED_OUTPUT_PORT |= _BV(PROBE_LED_PIN_BIT);
#endif
    return 1;
#else
#if OUTPUT_PROBE_LED_ENABLED > 0
    PROBE_LED_OUTPUT_PORT &= ~_BV(PROBE_LED_PIN_BIT);
#endif
    return 0;
#endif
  } else {
#if PROBE_INVERT == 0
#if OUTPUT_PROBE_LED_ENABLED > 0
    PROBE_LED_OUTPUT_PORT &= ~_BV(PROBE_LED_PIN_BIT);
#endif
    return 0;
#else
#if OUTPUT_PROBE_LED_ENABLED > 0
    PROBE_LED_OUTPUT_PORT |= _BV(PROBE_LED_PIN_BIT);
#endif
    return 1;
#endif
  }
}
#endif

void setup() {

  cli();
  sendTachData           = false;
  tickTimerFlag          = false;
  updateFrequencyCounter = 0;

// Initialize DRO values
#if DRO_ENABLED > 0
  // use clock only for scale type 0
#if DRO_TYPE0_ENABLED
  // clock pin should be set as output
  SCALE_CLK_DDR |= _BV(CLK_PIN_BIT);
  // set the clock pin to low
  SCALE_CLK_OUTPUT_PORT &= ~_BV(CLK_PIN_BIT);
#endif

  // data pins should be set as input for scale type 0 and as output for scale type 1
#if SCALE_X_ENABLED > 0
#if SCALE_X_TYPE == 0
  X_DDR &= ~_BV(X_PIN_BIT);
#elif SCALE_X_TYPE == 1
  X_DDR |= _BV(X_PIN_BIT);
#endif
  xValue         = 0L;
  xReportedValue = 0L;
#if SCALE_X_AVERAGE_ENABLED > 0
  initializeAxisAverage(axisLastReadX, axisLastReadPositionX, axisAMAValueX);
#endif
#endif

#if SCALE_Y_ENABLED > 0
#if SCALE_Y_TYPE == 0
  Y_DDR &= ~_BV(Y_PIN_BIT);
#elif SCALE_Y_TYPE == 1
  Y_DDR |= _BV(Y_PIN_BIT);
#endif
  yValue         = 0L;
  yReportedValue = 0L;
#if SCALE_Y_AVERAGE_ENABLED > 0
  initializeAxisAverage(axisLastReadY, axisLastReadPositionY, axisAMAValueY);
#endif
#endif

#if SCALE_Z_ENABLED > 0
#if SCALE_Z_TYPE == 0
  Z_DDR &= ~_BV(Z_PIN_BIT);
#elif SCALE_Z_TYPE == 1
  Z_DDR |= _BV(Z_PIN_BIT);
#endif
  zValue         = 0L;
  zReportedValue = 0L;
#if SCALE_Z_AVERAGE_ENABLED > 0
  initializeAxisAverage(axisLastReadZ, axisLastReadPositionZ, axisAMAValueZ);
#endif
#endif

#if SCALE_W_ENABLED > 0
#if SCALE_W_TYPE == 0
  W_DDR &= ~_BV(W_PIN_BIT);
#elif SCALE_W_TYPE == 1
  W_DDR |= _BV(W_PIN_BIT);
#endif
  wValue         = 0L;
  wReportedValue = 0L;
#if SCALE_W_AVERAGE_ENABLED > 0
  initializeAxisAverage(axisLastReadW, axisLastReadPositionW, axisAMAValueW);
#endif
#endif

// Initialize SPI and LS7366R registers for DRO type 1
#if DRO_TYPE1_ENABLED
  // SPI initialization
  SPI.begin();

#if SCALE_X_ENABLED > 0 && SCALE_X_TYPE == 1
  X_OUTPUT_PORT &= ~_BV(X_PIN_BIT);
  SPI.transfer(0x88);
  SPI.transfer(0x03);
  X_OUTPUT_PORT |= _BV(X_PIN_BIT);
#endif

#if SCALE_Y_ENABLED > 0 && SCALE_Y_TYPE == 1
  Y_OUTPUT_PORT &= ~_BV(Y_PIN_BIT);
  SPI.transfer(0x88);
  SPI.transfer(0x03);
  Y_OUTPUT_PORT |= _BV(Y_PIN_BIT);
#endif

#if SCALE_Z_ENABLED > 0 && SCALE_Z_TYPE == 1
  Z_OUTPUT_PORT &= ~_BV(Z_PIN_BIT);
  SPI.transfer(0x88);
  SPI.transfer(0x03);
  Z_OUTPUT_PORT |= _BV(Z_PIN_BIT);
#endif

#if SCALE_W_ENABLED > 0 && SCALE_W_TYPE == 1
  W_OUTPUT_PORT &= ~_BV(W_PIN_BIT);
  SPI.transfer(0x88);
  SPI.transfer(0x03);
  W_OUTPUT_PORT |= _BV(W_PIN_BIT);
#endif
#endif;

#endif

  // initialize tach values
#if TACH_ENABLED > 0
  // Setup tach port for input
  TACH_DDR &= ~_BV(TACH_PIN_BIT);

#if OUTPUT_TACH_LED_ENABLED > 0
  TACH_LED_DDR |= _BV(TACH_LED_PIN_BIT);
  // Set LED pin to LOW
  TACH_LED_OUTPUT_PORT &= ~_BV(TACH_LED_PIN_BIT);
#endif

  // Setup interrupt on tach pin
  PCICR |= _BV(TACH_INTERRUPT_REGISTER);
  TACH_INTERRUPT_MASK |= _BV(TACH_INTERRUPT_PIN);

  // Reset tach counter and timer
  tachInterruptRotationCount = 0;
  tachInterruptTimer         = micros();

  tachTimerStart             = tachInterruptTimer;

  tachReadoutRotationCount   = 0;
  tachReadoutMicrosec        = 0;

#if TACH_AVERAGE_COUNT > 1
  for (tachLastReadPosition = 0; tachLastReadPosition < (int)TACH_AVERAGE_COUNT;
       tachLastReadPosition++) {
    tachLastRead[tachLastReadPosition] = 0;
  }
  tachLastReadPosition = TACH_AVERAGE_COUNT - 1;
#endif
  tachUpdateFrequencyCounter = 0;

#endif

  // initialize touch probe values
#if PROBE_ENABLED > 0
  // Setup tach port for input
  PROBE_DDR &= ~_BV(PROBE_PIN_BIT);
#if OUTPUT_PROBE_LED_ENABLED > 0
  PROBE_LED_DDR |= _BV(PROBE_LED_PIN_BIT);
  // Set LED pin to LOW
  PROBE_LED_OUTPUT_PORT &= ~_BV(PROBE_LED_PIN_BIT);
#endif
  // Set probe input to "not touching"
  probeReportedValue = 0;

#endif

  // initialize serial port
  Serial.begin(UART_BAUD_RATE);

  // initialize timers
  setupClkTimer();

  sei();
}

int main() {
  setup();
  while (true) {
    if (tickTimerFlag) {
      tickTimerFlag = false;

#if DRO_ENABLED > 0
#if DRO_TYPE1_ENABLED
      readEncoders();
#endif
      // print DRO positions to the serial port
#if SCALE_X_ENABLED > 0
#if SCALE_X_AVERAGE_ENABLED > 0
      scaleValueRounded(xReportedValue, axisLastReadX, axisLastReadPositionX,
                        axisAMAValueX);
#endif
      Serial.print(F("X"));
      Serial.print((long)xReportedValue);
      Serial.print(F(";"));
#endif

#if SCALE_Y_ENABLED > 0
#if SCALE_Y_AVERAGE_ENABLED > 0
      scaleValueRounded(yReportedValue, axisLastReadY, axisLastReadPositionY,
                        axisAMAValueY);
#endif
      Serial.print(F("Y"));
      Serial.print((long)yReportedValue);
      Serial.print(F(";"));
#endif

#if SCALE_Z_ENABLED > 0
#if SCALE_Z_AVERAGE_ENABLED > 0
      scaleValueRounded(zReportedValue, axisLastReadZ, axisLastReadPositionZ,
                        axisAMAValueZ);
#endif
      Serial.print(F("Z"));
      Serial.print((long)zReportedValue);
      Serial.print(F(";"));
#endif

#if SCALE_W_ENABLED > 0
#if SCALE_W_AVERAGE_ENABLED > 0
      scaleValueRounded(wReportedValue, axisLastReadW, axisLastReadPositionW,
                        axisAMAValueW);
#endif
      Serial.print(F("W"));
      Serial.print((long)wReportedValue);
      Serial.print(F(";"));
#endif

#endif

      // print Tach rpm to serial port
#if TACH_ENABLED > 0

      // Calculate tach data
      sendTachData = sendTachOutputData() || sendTachData;

      // Check tach reporting frequency
      tachUpdateFrequencyCounter++;
      if (tachUpdateFrequencyCounter >= tachUpdateFrequencyCounterLimit) {
        tachUpdateFrequencyCounter = 0;

        // Output tach data
        if (sendTachData) {
          sendTachData = false;

          Serial.print(F("T"));
#if TACH_RAW_DATA_FORMAT > 0
          Serial.print((unsigned long)tachReadoutMicrosec);
          Serial.print(F("/"));
          Serial.print((unsigned long)tachReadoutRotationCount);
#else
          Serial.print((unsigned long)tachReadoutRpm);
#endif
          Serial.print(F(";"));
        }
      }
#endif

      // print Touch Probe data to serial port
#if PROBE_ENABLED > 0
      // Calculate tach data
      probeReportedValue = readProbeOutputData();

      Serial.print(F("P"));
      Serial.print((unsigned int)probeReportedValue);
      Serial.print(F(";"));
#endif
    }
  }
}