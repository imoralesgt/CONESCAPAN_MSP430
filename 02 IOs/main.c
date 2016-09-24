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

// LEDs en P1.0 y P4.7
// 0b00000001 = 0x01
// 0b10000000 = 0xF0

// Push-Button en P2.1
// 0b00000010 = 0x02

#include <msp430f5529.h>

const unsigned int MAX_DELAY = 20000;
unsigned int ledState = 0; //0 = LED Rojo, 1 = LED Verde
unsigned int pushState = 1; //0 = Presionado, 1 = Sin Presionar

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Detener el Watchdog Timer

	//------- Inicializar el pin correspondiente al LED rojo (P1.0)
	P1DIR |= 0x01;					// Establecer pin P1.0 como salida
	P1OUT &= ~0x01;					// Inicializar pin P1.0 para estado en bajo

	//------ Inicializar el pin correspondiente al LED verde (P4.7)
	P4DIR |= 0xF0;
	P4OUT &= ~0x01;

	//------- Inicializar el pin correspondiente al Push-Button S1 (P2.1)
	P2DIR &= ~0x02;					// Definir como entrada
	P2REN |= 0x02;					// Habilitar resistencia Pull-Up/Down
	P2OUT |= 0x02;					// Seleccionar resistencia Pull-Up

	while(1){
		volatile unsigned int i;	// variable volatil (permite optimizar uso de RAM)

		pushState = (P2IN & 0x02) >> 1; //Enmascaramiento y corrimiento


		if(pushState){ //if(pushState != 0)
			ledState = 0;
		}else{
			ledState = 1;
		}


		if(!ledState){ //if(state == 0)
			P1OUT ^= 0x01; //Toggle LED Rojo
			P4OUT &= ~0xF0; //Mantener LED Verde apagado
		}else{
			P4OUT ^= 0xF0; //Toggle LED Verde
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
 * 		  En vez de requerir que se mantenga presionado el Push-button,
 * 		  el usuario solamente tiene que presionar una vez para cambiar
 * 		  el LED que parpadea.
 */
