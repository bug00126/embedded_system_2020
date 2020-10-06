#include <msp430.h> 
/*#define init 0;
#define 1st_red_on  1
#define 1st_red_off 2
#define 2nd_red_on  3
#define 2nd_red_off 4
#define green_on    5
#define next_state(x) (++(x))%6*/

/*int next_state(int a){
    return (++a)%6;
}*/

int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= 0x41;
    TA0CCR0 |= 5999;
    TA0CTL |= MC_1|ID_0|TASSEL_1|TACLR;
    BCSCTL1 |= 0x80;
    BCSCTL2 |= SELM_3;
    BCSCTL3 |= LFXT1S_2;

    volatile unsigned int state = 0;

    for(;;){
        if(TA0CTL&TAIFG && state==0) {
            P1OUT |= 0x01;
            state++;
            TA0CTL &= ~TAIFG;
        } else if(TA0CTL&TAIFG && state==1) {
            P1OUT &= ~0x41;
            state++;
            TA0CTL &= ~TAIFG;
        } else if(TA0CTL&TAIFG && state==2) {
            P1OUT |= 0x01;
            state++;
            TA0CTL &= ~TAIFG;
        } else if(TA0CTL&TAIFG && state==3) {
            P1OUT &= ~0x41;
            state++;
            TA0CTL &= ~TAIFG;
        } else if(TA0CTL&TAIFG && state==4) {
            P1OUT |= 0x40;
            state++;
            TA0CTL &= ~TAIFG;
            TA0CCR0 = 10799;
        } else if(TA0CTL&TAIFG && state==5) {
            P1OUT &= ~0x41;
            state=0;
            TA0CTL &= ~TAIFG;
            TA0CCR0 = 5999;
        }
        /*switch(state) {
        case init:
            if(TA0CTL&TAIFG) {
                P1OUT |= 0x01;
                state = next_state(state);
            }
            break;
        case 1st_red_on:
            if(TA0CTL&TAIFG) {
                P1OUT &= ~0x41;
                state = next_state(state);
            }
            break;
        case 1st_red_off:
            if(TA0CTL&TAIFG) {
                P1OUT |= 0x01;
                state = next_state(state);
            }
            break;
        case 2nd_red_on:
            if(TA0CTL&TAIFG) {
                P1OUT &= ~0x41;
                state = next_state(state);
            }
            break;
        case 2nd_red_off:
            if(TA0CTL&TAIFG) {
                P1OUT |= 0x40;
                state = next_state(state);
                TA0CCR0 = 49999;
            }
            break;
        case green_on:
            if(TA0CTL&TAIFG) {
                P1OUT &= ~0x41;
                state = next_state(state);
                TA0CCR0 = 19999;
            }
            break;
        }*/
    }

    return 0;
}
