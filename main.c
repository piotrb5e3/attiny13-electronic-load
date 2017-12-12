#include <avr/io.h>
#include <util/delay.h>

/*
 * Connections:
 *   2 (PB3, ADC3) -> Actual current sense
 *   3 (PB4, ADC2) -> Set current sense
 *   5 (PB0)       -> DIO
 *   6 (PB1)       -> CLK
 *   7 (PB2)       -> LED
 */

/**
 *  I2C communication CONSTANTS
 */
#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

// Segments diagram
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
//
// Display is mounted upside-down

const uint8_t flipDigitToSegment[] = {
		// XGFEDCBA
		0b00111111,    // 0
		0b00110000,    // 1
		0b01011011,    // 2
		0b01111001,    // 3
		0b01110100,    // 4
		0b01101101,    // 5
		0b01101111,    // 6
		0b00111000,    // 7
		0b01111111,    // 8
		0b01111101,    // 9
};

// Delay for I2C communication
static inline void bit_delay() { _delay_us(50); }
// I2C communication operations
static inline void clk_high() { DDRB &= ~(1<<DDB1); }
static inline void clk_low() { DDRB |= 1<<DDB1; }
static inline void dio_high() { DDRB &= ~(1<<DDB0); }
#define dio_low() DDRB|=(1<<DDB0)
#define dio_read() (PORTB&(1<<PB0))!=0

// ADC channel selection
static inline void set_adc_read_actual_current() { ADMUX |= 0x03;}
static inline void set_adc_read_set_current() { ADMUX = (ADMUX & (~0x01)) | 0x02;}

// LED toggle
static inline void led_green() { PORTB &= ~(1<<PB2); }
static inline void led_red() { PORTB |= 1<<PB2; }

// Internal Vref in mV
#define VREF 1100
// Power resistor value in mOHM
#define R 500

// I2C START sequence
void stop() {
	dio_low();
	bit_delay();
	clk_high();
	bit_delay();
	dio_high();
	bit_delay();
}
//I2C STOP sequence
void start() {
	dio_low();
	bit_delay();
}

// Write byte over I2C
void write_byte(uint8_t b) {
	// 8 Data Bits
	for(uint8_t i = 0; i < 8; i++) {
		// CLK low
		clk_low();
		bit_delay();

		// Set data bit
		if (b & 0x01) {
			dio_high();
		} else {
			dio_low();
		}

		bit_delay();

		// CLK high
		clk_high();
		bit_delay();
		b >>= 1;
	}

	// Wait for acknowledge
	// CLK to zero
	clk_low();
	dio_high();
	bit_delay();

	// CLK to high
	clk_high();
	bit_delay();
	uint8_t ack = dio_read();
	if (ack == 0){
		dio_low();
	}

	bit_delay();
	clk_low();
	bit_delay();
}

// Set lit segments
static inline void set_segments(const uint8_t segments[4])
{
	// Write COMM1
	start();
	write_byte(TM1637_I2C_COMM1);
	stop();

	// Write COMM2
	start();
	write_byte(TM1637_I2C_COMM2);

	// Write the data bytes
	for (uint8_t k=0; k < 4; k++){
		write_byte(segments[k]);
	}

	stop();

	// Write COMM3 + brightness
	start();
	// 0x08 -> display on
	// 0x07 -> max brightness
	write_byte(TM1637_I2C_COMM3 + (0x08 | 0x07));
	stop();
}

// Display a 4 digit number on the display
static inline void show_number(uint16_t n) {
	uint8_t s[4];
	for (uint8_t i = 0; i<4; i++) {
		s[i] = flipDigitToSegment[n % 10];
		n /= 10;
	}
	set_segments(s);
}

// analog read from currently selected channel
static inline uint16_t analog_read() {
	// Start conversion
	ADCSRA |= 1<<ADSC;
	// Wait for the conversion to end
	while (!(ADCSRA & 1<<ADIF));
	// Read value
	uint16_t v = ADCL | (ADCH<<8);
	// Reset conversion complete interrupt flag
	ADCSRA |= 1<<ADIF;

	return v;
}

// Read voltage on selected pin and convert it to current in mA
uint16_t get_current() {
	uint32_t v;
	v = analog_read();
	v *= VREF;
	v *= 1000;
	v /= 1024;
	v /= R;
	return v;
}

int main() {
	// Set LED pin for output
	DDRB = 1<<DDB2;
	// Set LED pin to low
	PORTB = 0x0;
	// Use internal ADC reference
	ADMUX = 1<<REFS0 | 0<<ADLAR | 0<<MUX1 | 1<<MUX0;
	// Enable ADC with 32 prescaler
	ADCSRA = 1<<ADEN | 1<<ADPS2 | 0<<ADPS1 | 1<<ADPS0;

	uint8_t reading_actual = 1;

	while(1) {
		if(reading_actual){
			led_green();
			set_adc_read_actual_current();
		} else {
			led_red();
			set_adc_read_set_current();
		}
		for(uint8_t i = 0; i<20; i++) {
			show_number(get_current());
			_delay_ms(300);
		}
		reading_actual = !reading_actual;
	}
}
