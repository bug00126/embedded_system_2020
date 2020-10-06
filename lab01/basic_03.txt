#include <msp430.h>
#define B1 BIT3
int main(void){
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= 0x41;
	P1OUT |= 0x01;
	P1REN |= B1;
	P1OUT |= B1;

	for(;;){
	    volatile unsigned int i;

	    if ((P1IN & B1)==0) {
	        i = 10000;
	        while (i>0) {
	            i--;
	            if (i>5000) P1OUT |= 0x41;
	            else P1OUT &= ~0x41;
	        }
	    } else {
	        i = 20000;
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
	        }


	    }
	}
	
	return 0;
}
