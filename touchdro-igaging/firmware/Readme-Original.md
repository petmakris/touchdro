NOTE: This program supports pulse sensor to measure rpm and switch type touch probe.  
For quadrature encoder scales use LS7366R-based shield.

If at least one quadrature scale is used do not conect other devices to SPI dedicated pins
as LS7366R uses SPI for communication

Configuration parameters:

- `SCALE_<n>_ENABLED`

Defines if DRO functionality on axis <n> should be supported.
If supported DRO scale should be connected to I/O pin defined in constant "<n>DataPin" and
DRO data is sent to serial port with corresponding axis prefix (X, Y, Z or W)
Clock pin is common for all iGaging scales should be connected to I/O pin defined in constant "clockPin"
Possible values:
    0 = DRO functionality on axis <n> is not supported
    1 = DRO functionality on axis <n> is supported
Default value = 1

- `SCALE_<n>_TYPE`

Defines the type of scale used on axis <n>.
Two types of scales are supported: iGaging/AccuRemote Digital Scales and  quadrature encoder scales (common glass or magnetic)
Note: If at least on scale is type 1 do not connect any other devices to SPI pins as it will interfere with communication with LS7366R.
Possible values:
    0 = iGaging/AccuRemote Digital Scales with 21bit protocol
    1 = Quadrature Encoder scales through LS7366R-type shield (32-bit quadrature counter with serial interface).
Default value = 0

- `SCALE_CLK_PIN`

Defines the I/O pin where clock signal for all iGaging DRO scales is connected.  Used only if at least one scale is type 0.
Possible values:
    integer number between 2 and 13
Default value = 2

- `SCALE_<n>_PIN`

Defines the I/O pin where DRO data signal for selected scale is connected
Note: For quadrature scale this pin is connected to SPI SS pin in corresponding LS7366R.
Possible values:
    integer number between 2 and 13
Default values = 3, 4, 5, 6 (for corresponding axis X, Y, Z and W)

- `SCALE_<n>_AVERAGE_ENABLED`

Defines if DRO reading should be averaged using weighted average calculation with automating smoothing factor.
If average is enabled the reading is much more stable without "jumping" and "flickering" when the scale "can't decide" on the value.
Note: This value is not used when corresponding SCALE_<n>_ENABLED is 0
Possible values:
    0 = exact measured from the scale is sent
    1 = scale reading averaged using weighted average calculation with automatic smoothing factor
Default value = 1

- `AXIS_AVERAGE_COUNT`

Defines the number of last DRO readings that will be used to calculate weighted average for DRO.
For machines with power feed on any axis change this value to lower number i.e. 8 or 16.
Possible values:
    integer number between 4 and 32
Recommended values:
    16 for machines with power feed
    32 for all manual machines
Default value = 24

- `TACH_ENABLED`

Defines if tach sensor functionality should be supported.
If supported tach sensor should be connected to I/O pin defined in constant INPUT_TACH_PIN and
rpm value is sent to serial port with axis prefix "T"
Possible values:
    0 = tach sensor functionality is not supported
    1 = tach sensor functionality is supported
Default value = 1

- `INPUT_TACH_PIN`

Defines the I/O pin where tach sensor signal is connected
Possible values:
    integer number between 2 and 13
Default value = 7

- `TACH_PRESCALE`

Defines how many tach pulses per one revolution the sensor sends.
For example if tach sensor uses two magnets on the shaft the sensor will generate two pulses per revolution.
This can be used to get better resolution and faster response time for very low rpm
Note: This value is not used when TACH_RAW_DATA_FORMAT is enabled
Possible values:
    any integer number greater than 0
Default value = 1

- `TACH_AVERAGE_COUNT`
  
Defines the number of last tach readings that will be used to calculate average tach rpm.
If you want to send measured rpm instead of average rpm set this value to 1.
Note: This value is not used when TACH_RAW_DATA_FORMAT is enabled.
        It is recommended to set this value 2 times or more of TACH_PRESCALE value.
        For example: if TACH_PRESCALE = 4, set TACH_AVERAGE_COUNT = 8
Possible values:
    1 = exact measured tach reading is sent
    any integer number greater than 1 - average tach reading is sent
Default value = 6

- `TACH_ROUND`
    Defines how tach reading should be rounded. If rounding is enabled the reading can be rounded either by 1% of current rpm or to the fixed "round" number with predefined RPM thresholds ("smart rounding"). For example with 1% rounding if measured rpm is between 980rpm and  1020 rpm the display will show numbers rounded to 9 and 10 (i.e. 981, 990, 999, 1000, 1010, 1020 etc.). With "smart rounding" the measured rpm is rounded to the nearest 1, 2, 5, 10, 20, 50 and 100 depends on measured RPM (change at predefined thresholds). For example with "smart rounding" all measured rpm is between 500pm and  2000 rpm the display will show numbers rounded to the nearest 5 (i.e. 980, 985, 990, 995, 1000, 1005  etc.). Note: This value is not used when TACH_RAW_DATA_FORMAT is enabled Possible values: 0 = exact measured tach reading is sent 1 = tach reading is rounded to the nearest 1% of measured rpm (1% rounding) 2 = tach reading is rounded to the nearest "round" number with fixed thresholds ("smart rounding") Default value = 2

- `TACH_RAW_DATA_FORMAT`
    Defines the format of tach data sent to serial port.
    Note: when raw data format is used, then TACH_PRESCALE, TACH_AVERAGE_COUNT and TACH_ROUND are ignored
    Possible values:
        0 = tach data is sent in single value format: T<rpm>;
        1 = tach data is sent in raw (two values) format: T<total_time>/<number_of_pulses>;
    Default value = 0

- `MIN_RPM_DELAY`
    Defines the delay (in milliseconds) in showing 0 when rotation stops.  If rpm is so low and time between tach pulse
    changes longer than this value, value zero rpm ("T0;") will be sent to the serial port.
    Note: this number will determine the slowest rpm that can be measured.  In order to measure smaller rpm I suggest
            to use a sensor with more than one "ticks per revolution" (for example hall sensor with two or more magnets).
            The number of "ticks per revolution" should be set in tachometer setting in Android app.
    Possible values:
        any integer number greater than 0
    Default value = 1200 (the minimum rpm measured will be 50 rpm)

- `OUTPUT_TACH_LED_ENABLED`
    Defines if the tach LED feedback is supported.
    If supported the tach feedback LED should be connected to I/O pin defined in constant OUTPUT_TACH_LED_PIN below
    Possible values:
        0 = tach LED feedback functionality is not supported
        1 = tach LED feedback functionality is supported
    Default value = 1

- `OUTPUT_TACH_LED_PIN`
    Defines the I/O pin where the tach LED feedback is connected.
    Tach LED feedback indicates the status of INPUT_TACH_PIN for debugging purposes
    Possible values:
        integer number between 2 and 13
    Default value = 9

- `PROBE_ENABLED`
    Defines if touch probe sensor functionality should be supported.
    If supported touch probe should be connected to I/O pin defined in constant INPUT_PROBE_PIN.
    Possible values:
        1 = touch probe functionality is supported
        0 = touch probe functionality is not supported
    Default value = 1

- `INPUT_PROBE_PIN`
    Defines the I/O pin where touch probe signal is connected
    Possible values:
        integer number between 2 and 13
    Default value = 8

- `PROBE_INVERT`
    Defines if the touch probe input pin signal needs to be inverted (enter the signal level when touch probe is not touching).
    Possible values:
        0 = touch probe input pin signal is LOW (logical Zero) when touch probe is in "normal open" status (not touching)
        1 = touch probe input pin signal is HIGH (logical One) when touch probe is in "normal open" status (not touching)
    Default value = 0

- `OUTPUT_PROBE_LED_ENABLED`
    Defines if the touch probe LED feedback is supported.
    If supported the touch probe feedback LED should be connected to I/O pin defined in constant INPUT_PROBE_PIN below
    Possible values:
        1 = touch probe LED feedback functionality is supported
        0 = touch probe LED feedback functionality is not supported
    Default value = 1

- `OUTPUT_PROBE_LED_PIN`
    Defines the I/O pin where the touch probe LED feedback is connected.
    Touch probe LED feedback indicates the status of INPUT_PROBE_PIN for debugging purposes
    Possible values:
        integer number between 2 and 13
    Default value = 10

- `UART_BAUD_RATE`
    Defines the serial port baud rate.  Make sure it matches the Bluetooth module's baud rate.
    Recommended value:
        1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
    Default value = 9600

- `UPDATE_FREQUENCY`
    Defines the Frequency in Hz (number of timer per second) the scales are read and the data is sent to the application.
    Possible values:
        any integer number between 1 and 64
    Default value = 24

- `TACH_UPDATE_FREQUENCY`
    Defines the max Frequency in Hz (number of timer per second) the tach output is sent to the application.
    Note: This value must be a divider of UPDATE_FREQUENCY that would result zero reminder.
            For example for UPDATE_FREQUENCY = 24 valid TACH_UPDATE_FREQUENCY are: 1, 2, 3, 4, 6, 8, 12 and 24
    Possible values:
        any integer number between 1 and UPDATE_FREQUENCY
    Default value = 4
