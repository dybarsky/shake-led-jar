#include <msp430g2553.h>
#include "sensor.h"

#define SENSOR_PIN BIT7

void configure_sensor() {
	// select as digital io
	P2SEL &= ~SENSOR_PIN;
	// set input mode
	P2DIR &= ~SENSOR_PIN;
	// enable pull resistor
	P2REN |= SENSOR_PIN;
	// pull down
	P2OUT &= ~SENSOR_PIN;
	// interruption from 0 to 1
	P2IES &= ~SENSOR_PIN;
	// reset interruption flag
	P2IFG &= ~SENSOR_PIN;
	// enable interruption
	P2IE |= SENSOR_PIN;
}

#pragma vector = PORT2_VECTOR
__interrupt void on_port2_high() {
	if (P2IFG & SENSOR_PIN) {
		// reset intrrution flag
		P2IFG &= ~SENSOR_PIN;
		// invoke callback
		on_sensor_callback();
	}
}
