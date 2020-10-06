#include <msp430.h>
#define B1 BIT3
int main(void){
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= 0x41;
	P1OUT |= 0x01;
	P1REN |= B1;
	P1OUT |= B1;
	TA0CCR0 |= 19999;
	TA0CTL |= MC_1|ID_3|TASSEL_2|TACLR;
	int isOn = 0, state = 0, time = 0;

	for(;;){
	    volatile unsigned int i;

	    if ((P1IN & B1)==0) {
	        if (TA0CTL&TAIFG) {
	            if(isOn==0) {
	                P1OUT |= 0x41;
	                TA0CTL &= ~TAIFG;
	                isOn = 1;
	            } else {
	                P1OUT &= ~0x41;
	                TA0CTL &= ~TAIFG;
	                isOn = 0;
	            }
	        }
	        /*i = 10000;
	        while (i>0) {
	            i--;
	            if (i>5000) P1OUT |= 0x41;
	            else P1OUT &= ~0x41;
	        }*/
	    } else {
	        if (TA0CTL&TAIFG) {
	            if(state==0){
	                if (!isOn) {
	                    P1OUT |= 0x01;
	                    TA0CTL &= ~TAIFG;
	                    isOn = !isOn;
	                } else {
	                    P1OUT &= ~0x01;
	                    TA0CTL &= ~TAIFG;
	                    isOn = !isOn;
	                    state++;
	                }
	            } else if(state==1) {
	                if (!isOn) {
	                    P1OUT |= 0x01;
	                    TA0CTL &= ~TAIFG;
	                    isOn = !isOn;
	                } else {
	                    P1OUT &= ~0x01;
	                    TA0CTL &= ~TAIFG;
	                    isOn = !isOn;
	                    state++;
	                }
	            } else {
	                if (!isOn) {
	                    if (!time) {
	                        P1OUT |= 0x40;
	                        TA0CTL &= ~TAIFG;
	                        time = !time;
	                    } else {
	                        isOn = !isOn;
	                        TA0CTL &= ~TAIFG;
	                        time = !time;
	                    }
	                } else {
	                    if (!time) {
	                        P1OUT &= ~0x40;
	                        TA0CTL &= ~TAIFG;
	                        time = !time;
	                    } else {
	                        isOn = !isOn;
	                        TA0CTL &= ~TAIFG;
	                        time = !time;
	                        state = 0;
	                    }
	                }
	            }
	        }
	        /*i = 20000;
	        while (i>0) {
	            i--;
	            if (i>15000 || (i <= 10000 && i > 5000)) P1OUT |= 0x01;
	            else P1OUT &= ~0x01;
	        }

	        i = 20000;
	        while (i>0) {
	            i--;
	            if (i>10000) P1OUT |= 0x40;
	            else P1OUT &= ~0x40;
	        }*/

	    }
	}
	
	return 0;
}
