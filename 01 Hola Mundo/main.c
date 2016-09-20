/*
 * CONESCAPAN 2016
 * TALLER DE INTRO. A MICROCONTROLADORES
 * TI MSP430
 * GUATEMALA, SEPTIEMBRE 2016
 * IVAN RENE MORALES
*/

// ---------------------------------------------

// DCO sourcing MCLK & SMCLK @ Aprox 1.1 MHz
//
// Convencion de nomenclatura
//	- Variables: variableUno
//  - Constantes: CONSTANTE_UNO

#include <msp430g2553.h>

const unsigned int MAX_DELAY = 10000;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Detener el Watchdog Timer
	P1DIR |= 0x01;					// Establecer el pin P1.0 como salida
	P1OUT &= ~0x01;					// Inicializar el pin P1.0 en estado bajo

	while(1){
		volatile unsigned int i;	// variable volatil (permite optimizar uso de RAM)

		P1OUT ^= 0x01;				// Aplicar operación XOR en pin P1.0 (Toggle)

		i = MAX_DELAY;					// Retardo por software
		while(--i != 0);
	}
	
	return 0;
}
