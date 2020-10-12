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
#define _counting    8
#define _pressed     1
#define _released    0

int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= 0x41;
    P1OUT |= 0x01;
    P1OUT |= B1;
    P1REN |= B1;
    TA0CTL |= MC_1|ID_0|TASSEL_1|TACLR;
    TA1CTL |= ID_3|TASSEL_2|TACLR;
    TA0CCR0 |= 5999;
    TA1CCR0 |= 31249;
    DCOCTL   = CALDCO_1MHZ;
    BCSCTL1  = CALBC1_1MHZ;
    BCSCTL2 |= DIVS_3;                      // smclk divider set to 8
    BCSCTL3 |= LFXT1S_2;                    // set aclk to vlo

    volatile unsigned int state = 1, event = 0, next_state = 1;

    for(;;){

        if ((P1IN & B1)==0) event = _pressed;
        else event = _released;

        if(TA0CTL&TAIFG) {      // handle state change
            TA0CTL &= ~TAIFG;
            state = next_state;
        }

        switch (state) {
        case _init :
            switch (event) {
            case _pressed :
                state = _counting;
                break;
            case _released:
                TA0CCR0 = 5999;
                next_state = _1st_red_on;
                P1OUT &= ~0x41;
                break;
            }
            break;
        case _1st_red_on :
            switch (event) {
            case _pressed :
                state = _counting;
                break;
            case _released:
                next_state = _1st_red_off;
                P1OUT |= 0x01;
                break;
            }
            break;
        case _1st_red_off :
            switch (event) {
            case _pressed :
                state = _counting;
                break;
            case _released:
                next_state = _2nd_red_on;
                P1OUT &= ~0x41;
                break;
            }
            break;
        case _2nd_red_on :
            switch (event) {
            case _pressed :
                state = _counting;
                break;
            case _released:
                next_state = _2nd_red_off;
                P1OUT |= 0x01;
                break;
            }
            break;
        case _2nd_red_off :
            switch (event) {
            case _pressed :
                state = _counting;
                break;
            case _released:
                next_state = _green_on;
                P1OUT &= ~0x41;
                break;
            }
            break;
        case _green_on :
            switch (event) {
            case _pressed :
                TA0CCR0 = 5999;
                state = _counting;
                break;
            case _released:
                next_state = _init;
                P1OUT |= 0x40;
                TA0CCR0 = 10799;
                break;
            }
            break;
        case _both_on :
            switch (event) {
            case _pressed :
                next_state = _both_off;
                P1OUT |= 0x41;
                break;
            case _released:
                state = _1st_red_on;
                TA0CTL |= TACLR;
                P1OUT &= ~0x41;
                break;
            }
            break;
        case _both_off :
            switch (event) {
            case _pressed :
                next_state = _both_on;
                P1OUT &= ~0x41;
                break;
            case _released:
                state = _1st_red_on;
                TA0CTL |= TACLR;
                P1OUT &= ~0x41;
                break;
            }
            break;
        case _counting:
            switch (event) {
            case _pressed :
                P1OUT |= 0x41;
                TA0CTL &= ~MC_1;
                TA1CTL |= MC_1;

                if (TA1CTL&TAIFG) {
                    state = _both_off;
                    TA1CTL &= ~TAIFG;
                    TA1CTL &= ~MC_1;
                    TA0CTL |= MC_1|TACLR;
                }
                break;
            case _released:
                state = _1st_red_on;
                TA0CTL |= MC_1|TACLR;
                P1OUT &= ~0x41;
                break;
            }
            break;
        }

    }

    return 0;
}
