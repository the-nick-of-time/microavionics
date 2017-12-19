#include <p18cxxx.h>
#include <delays.h>
// #include <float.h>
#include "ADCread.h"

// 2.5 microsecond delay, 10 clock cycles
#define TACQ 1


float abs(float x){
	// bit masking would also work but that would be less general and harder to understand
	return (x<0) ? -x : x;
}


bool startup_ADC(void) {
	// Set up analog to digital conversion
	// Default reference voltages
	ADCON1 = 0x00;
	// Right justified, Tacq = 4 Tad, 1/Tad = Fosc/16
	ADCON2 = 0b10010101;
	// Low priority, not interrupting yet, enabled
	PIR1bits.ADIF = 0;
	return true;
}


int pot_ADC(void){
	// Potentiometer is on channel 0
	ADCON0bits.CHS = 0b00000;
	// Start to charge the acquire capacitor
	ADCON0bits.ADON = 1;
	// Delay to charge it fully
	Delay10TCYx(TACQ);
	// Start the conversion
	ADCON0bits.GO = 1;
	// Wait until done
	while (!PIR1bits.ADIF){}
	PIR1bits.ADIF = 0;
	// Turn back off and return value
	ADCON0bits.ADON = 0;
	return ADRES;
}
