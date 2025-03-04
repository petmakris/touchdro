#include <Arduino.h>

#define cbi(sfr, bit)         (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit)         (_SFR_BYTE(sfr) |= _BV(bit))

#define CLK_PIN_BIT           2
#define SCALE_CLK_DDR         DDRD
#define SCALE_CLK_OUTPUT_PORT PORTD

#define X_PIN_BIT             3
#define X_DDR                 DDRD
#define X_INPUT_PORT          PIND

#define UART_BAUD_RATE        9600

//  Frequency in Hz (number of timer per second the scales are read and the data is sent to the application)
#define UPDATE_FREQUENCY      24

// iGaging and Accuremote scales use 21 bit format
#define SCALE_CLK_PULSES      21

// iGaging scales run at about 9-10KHz
#define SCALE_CLK_FREQUENCY   9000

// iGaging scales clock run at 20% PWM duty (22us = ON out of 111us cycle)
#define SCALE_CLK_DUTY        20

int const updateFrequencyCounterLimit = (int) (((uint64_t) SCALE_CLK_FREQUENCY) / ((uint64_t) UPDATE_FREQUENCY));
int const clockCounterLimit           = (int) (((uint64_t) (F_CPU / 8)) / (uint64_t) SCALE_CLK_FREQUENCY) - 10;
int const scaleClockDutyLimit         = (int) (((uint64_t) (F_CPU / 800)) * ((uint64_t) SCALE_CLK_DUTY) / (uint64_t) SCALE_CLK_FREQUENCY);
int const scaleClockFirstReadDelay    = (int) ((uint64_t) F_CPU / 4000000);

// variables that will store the DRO readout
volatile bool tickTimerFlag;
volatile int  updateFrequencyCounter;

// Axis count

volatile long xValue;
volatile long xReportedValue;

void setupClkTimer() {
    updateFrequencyCounter = 0;

    TCCR2A                 = 0;
    TCCR2B                 = 0;
    OCR2A                  = scaleClockDutyLimit;  // default 44 = 22us
    OCR2B                  = clockCounterLimit;    // default 222 = 111us

    // turn on Fast PWM mode
    TCCR2A |= (1 << WGM21) | (1 << WGM20);

    // Set CS21 bit for 8 prescaler // CS20 for no prescaler
    TCCR2B |= (1 << CS21);

    // initialize counter value to start at low pulse
    TCNT2 = scaleClockDutyLimit + 1;

    // enable timer compare interrupt A and B
    TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B);
}

// Timer 2 interrupt B (Switches clock pin from low to high 21 times) at the end of clock counter limit
ISR(TIMER2_COMPB_vect) {
    TCNT2 = 0;

    // Only set the clock high if updateFrequencyCounter less than 21
    if (updateFrequencyCounter < SCALE_CLK_PULSES) {
        // Set clock pin high
        SCALE_CLK_OUTPUT_PORT |= (1 << CLK_PIN_BIT);
    }
}

// Timer 2 interrupt A ( Switches clock pin from high to low) at the end of clock PWM Duty counter limit
ISR(TIMER2_COMPA_vect) {

    // Control the scale clock for only first 21 loops
    if (updateFrequencyCounter < SCALE_CLK_PULSES) {

        // Set clock low if high and then delay 2us
        if (SCALE_CLK_OUTPUT_PORT & (1 << CLK_PIN_BIT)) {
            SCALE_CLK_OUTPUT_PORT &= ~(1 << CLK_PIN_BIT);
            TCNT2 = scaleClockDutyLimit - scaleClockFirstReadDelay;
            return;
        }

        // read the pin state and shift it into the appropriate variables
        // Logic by Les Jones:
        //	If data pin is HIGH set bit 20th of the axis value to '1'.  Then shift axis value one bit to the right
        //  This is called 20 times (for bits received from 0 to 19)
        if (updateFrequencyCounter < SCALE_CLK_PULSES - 1) {

            if (X_INPUT_PORT & (1 << X_PIN_BIT)) {
                xValue |= ((long) 0x00100000);
            }
            xValue >>= 1;

        } else if (updateFrequencyCounter == SCALE_CLK_PULSES - 1) {

            // If 21-st bit is 'HIGH' inverse the sign of the axis readout

            if (X_INPUT_PORT & (1 << X_PIN_BIT))
                xValue |= ((long) 0xfff00000);
            xReportedValue = xValue;
            xValue         = 0L;

            // Tell the main loop, that it's time to sent data
            tickTimerFlag  = true;
        }
    }

    updateFrequencyCounter++;
    // Start of next cycle
    if (updateFrequencyCounter >= updateFrequencyCounterLimit) {
        updateFrequencyCounter = 0;
    }
}

void setup() {

    cli();

    tickTimerFlag          = false;
    updateFrequencyCounter = 0;

    // clock pin should be set as output
    SCALE_CLK_DDR |= (1 << CLK_PIN_BIT);
    // set the clock pin to low

    SCALE_CLK_OUTPUT_PORT &= ~(1 << CLK_PIN_BIT);

    X_DDR &= ~(1 << X_PIN_BIT);
    xValue         = 0L;
    xReportedValue = 0L;

    setupClkTimer();

    Serial.begin(UART_BAUD_RATE);
    sei();
}

int main() {
    setup();
    while (true) {
        if (tickTimerFlag) {
            tickTimerFlag = false;

            Serial.print(F("X"));
            Serial.print((long) xReportedValue);
            Serial.print(F(";"));
        }
    }
}