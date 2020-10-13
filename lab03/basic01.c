#include <msp430.h>
#define B1 BIT3
#define _init 0
#define _1st_red_on  1
#define _1st_red_off 2
#define _2nd_red_on  3
#define _2nd_red_off 4
#define _green_on    5
#define _both_on     6
#define _both_off    7

volatile unsigned int state, next_state, press=0;

void init() {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= 0x41;
    P1OUT |= 0x01;
    P1OUT |= B1;
    P1REN |= B1;
    P1IE  |= B1;
    P1IES |= B1;
    P1IFG &= ~B1;
    TA0CTL |= MC_1|ID_0|TASSEL_1|TACLR;
    TA0CCR0 |= 5999;
    TA0CCTL0 = CCIE;
    BCSCTL3 |= LFXT1S_2;
    __enable_interrupt();
    state = 1;
    next_state = 1;
}

void update() {
    switch (state) {
    case _init :
        TA0CCR0 = 5999;
        next_state = _1st_red_on;
        P1OUT &= ~0x41;
        break;
    case _1st_red_on :
        next_state = _1st_red_off;
        P1OUT |= 0x01;
        break;
    case _1st_red_off :
        next_state = _2nd_red_on;
        P1OUT &= ~0x41;
        break;
    case _2nd_red_on :
        next_state = _2nd_red_off;
        P1OUT |= 0x01;
        break;
    case _2nd_red_off :
        next_state = _green_on;
        P1OUT &= ~0x41;
        break;
    case _green_on :
        next_state = _init;
        P1OUT |= 0x40;
        TA0CCR0 = 10799;
        break;
    case _both_on :
        next_state = _both_off;
        P1OUT |= 0x41;
        break;
    case _both_off :
        next_state = _both_on;
        P1OUT &= ~0x41;
        break;
    }
}

void main(void){
    init();
    while(1);
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TA0_ISR() {
    state = next_state;
    update();
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1() {
    if (!press) {
        state = _both_on;
        press = 1;
    } else {
        state = _1st_red_on;
        press = 0;
    }
    TA0CCR0 = 5999;
    TA0CTL |= TACLR;
    P1OUT &= ~0x41;
    P1IFG &= ~B1;
    update();
}
