#include <msp430g2553.h>
#include "led.h"
#include "timer.h"
#include "sensor.h"

// LED
#define ON 1
#define OFF 0

// mode
#define OPERATE 0x1
#define STANDBY 0x2

unsigned char led;
unsigned char mode;

void configure() {
	// turn off watchdog
	WDTCTL = WDTPW + WDTHOLD;
	// set frequency 1MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	// set all pins off
	P1OUT = 0;
	P2OUT = 0;
}

int main(void) {
	// init state
	led = OFF;
	mode = OPERATE;
	// config peripherals
	configure();
	configure_led();
	configure_timer();
	configure_sensor();
	// low power mode + enable interruptions
	_BIS_SR(LPM0_bits + GIE);
}

void on_sensor_callback() {
	// skip if not operating
	if (mode == STANDBY) {
		return;
	}
	// invert state
	switch (led) {
		case ON:
			led = OFF; 
			break;
		case OFF: 
			led = ON; 
			break;
	}
	// toggle led
	if (led == ON) {
		turn_on_led();
	} else {
		turn_off_led();
	}
	// distinct callback
	mode = STANDBY;
	start_timer();
}

void on_timer_callback() {
	mode = OPERATE;
	stop_timer();
}
