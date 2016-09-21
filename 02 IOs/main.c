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

// LEDs en P1.0 y P1.6
// 0b01000001 = 0x41

// Push-Button en P1.3
// 0b00001000 = 0x08

#include <msp430g2553.h>

const unsigned int MAX_DELAY = 20000;
unsigned int ledState = 0; //0 = LED Rojo, 1 = LED Verde
unsigned int pushState = 1; //0 = Presionado, 1 = Sin Presionar

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Detener el Watchdog Timer
	P1DIR |= 0x41;					// Establecer pine P1.0 y P1.6 como salida
	P1OUT &= ~0x41;					// Inicializar pines P1.0 y P1.6 en estado bajo

	P1DIR &= ~0x08;					// Definir como entrada
	P1REN |= 0x08;					// Habilitar resistencia Pull-Up/Down
	P1OUT |= 0x08;					// Seleccionar resistencia Pull-Up

	while(1){
		volatile unsigned int i;	// variable volatil (permite optimizar uso de RAM)

		pushState = (P1IN & 0x08) >> 3; //Enmascaramiento y corrimiento


		if(pushState){ //if(pushState != 0)
			ledState = 0;
		}else{
			ledState = 1;
		}


		if(!ledState){ //if(state == 0)
			P1OUT ^= 0x01; //Toggle LED Rojo
			P1OUT &= ~0x40; //Mantener LED Verde apagado
		}else{
			P1OUT ^= 0x40; //Toggle LED Verde
			P1OUT &= ~0x01; //Mantener LED Rojo apagado
		}

		i = MAX_DELAY;					// Retardo por software
		while(--i != 0);

	}
	
	return 0;
}

/*
 * EJERCICO:
 * 	Modificar el código para cumplir el siguiente objetivo:
 * 		- En vez de requerir que se mantenga presionado el Push-button,
 * 		  el usuario solamente tiene que presionar una vez para cambiar
 * 		  el LED que parpadea.
 */

