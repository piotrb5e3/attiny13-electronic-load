## ATtiny 13A electronic load
This is a simple, adjustable constant current electronic load
The ATtiny MCU is used to measure set and real current, and display it on a 7-segment display.
Current is set in the range of 0-2A through a precision potentiometer.
The maximum safe power depends on the radiator used.

## Requirements
* avr-gcc
* avr-libc
* an avr programmer

## Building
`cd Release`
`make`
OR
copy the contents of `main.c` to your IDE of choice

## Flashing
Depends on the programmer used

## Schematic
Possible changes:
* Op-amp - must be able to output close to the ground and work from 12V
* MOSFET - any N channel that can be driven from ~9V. Check drain-source voltage, maximum power dissipation and maximum continous drain current.
* Fan and radiator - for higher max power
* Polarity protection diodes
* Transient suppression diode
* D6 and D7 can be exchanged for a different 1.1V - 1.2V reference

## License
Both code and the schematic are MIT licensed
