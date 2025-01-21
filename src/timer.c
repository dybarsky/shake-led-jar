/*
frequency = 1 MHz
divider = 4
2 seconds = 500 000 tacts
timer counter = 0xFFFF
overflow = 8 cycles (tacts / counter)
*/

#include <msp430g2553.h>
#include "timer.h"

#define OVERFLOW 8
#define COUNT 0xFFFF

unsigned char cycles = 0;

void configure_timer() {
    // set timer counter
    TA1CCR0 = COUNT;
}

void start_timer() {
    // sum-main clock + freq divider 4 + up mode + init
    TA1CTL = TASSEL_2 + ID_2 + MC_1 + TACLR;
	// enable timer interruptions
    TA1CCTL0 = CCIE;
}

void stop_timer() {
	// disable timer interruptions 
	TA1CCTL0 = ~CCIE;
	// stop timer
	TA1CTL &= ~MC_3;
	// set stop mode
	TA1CTL |= MC_0;
	// reset overflow
	cycles = 0;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void on_timer1_ccr0(void) {
	// reset interruption flag
    TA1CCTL0 &= ~CCIFG;
	// skip cycle till full cycle completes
	if (++cycles < OVERFLOW) {
		return;
	} else {
		cycles = 0;
	}
	// invoke callback
	on_timer_callback();
}
