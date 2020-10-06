#include <msp430.h> 

int main(void){
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= 0x41;

	for(;;){
	    volatile unsigned int i;

	    i = 20000;
	    while (i>0) {
	        i--;
	        if (i>15000 || (i <= 10000 && i > 5000)) P1OUT |= 0x01;
	        else P1OUT &= 0;
	    }

	    /*P1OUT ^= 0x01;
	    i = 10000;
	    do i--;
	    while (i!=0);*/

	    i = 20000;
	    while (i>0) {
	        i--;
	        if (i>10000) P1OUT |= 0x40;
	        else P1OUT &= 0;
	    }
	}
	
	return 0;
}
