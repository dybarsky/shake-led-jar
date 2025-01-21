#include <msp430g2553.h>
#include "led.h"

#define LED_PIN BIT4

void configure_led() {
    P2DIR |= LED_PIN;
}

void turn_on_led() {
    P2OUT |= LED_PIN;
}

void turn_off_led() {
    P2OUT &= ~LED_PIN;
}
