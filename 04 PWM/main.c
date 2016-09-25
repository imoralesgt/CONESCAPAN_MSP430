/*
 * CONESCAPAN 2016
 * TALLER DE INTRO. A MICROCONTROLADORES
 * TI MSP430
 * GUATEMALA, SEPTIEMBRE 2016
 * IVAN RENE MORALES
*/

// ---------------------------------------------

// DCO sourcing MCLK & SMCLK @ 1.1 MHz
//
// Convencion de nomenclatura
//	- Variables: variableUno
//  - Constantes: CONSTANTE_UNO

// Salida PWM en P1.2
// 0b00000100 = 0x04

// Push-Button S1 en P2.1
// 0b00000010 = 0x02

// Push-Button S2 en P1.1
// 0b00000010 = 0x02

#include <msp430f5529.h>


//Otra forma para definir constantes, usando #define
//#define PWM_PERIOD_CCR 65535 // 1.1 MHz/65535 = 17 Hz (aprox)
#define PWM_PERIOD_CCR 220 // 1.1 MHz/220 = 5 kHz
#define PWM_MODIFY_STEP (PWM_PERIOD_CCR/50)  // Duty cycle step size (50 steps)

volatile unsigned int pwmDutyPeriod; //Variable modificada por interaccion externa al uC
unsigned int newPWM;

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Detener el Watchdog Timer

	// ------ Inicializar el pin correspondiente a salida de PWM (1.2)
	P1DIR |= 0x04;		  //Salida en pin de PWM (P1.2)
	P1SEL |= 0X04;		  //Special function select (P1.2)
	P1OUT &= ~0x04;		  //Definir estado inicial bajo

	// ------- Inicializar el pin correspondiente al Push-Button S1 (P2.1)
	P2DIR &= ~0x02;			// Definir como entrada
	P2REN |= 0x02;			// Habilitar resistencia Pull-Up/Down
	P2OUT |= 0x02;			// Seleccionar resistencia Pull-Up


	// ------- Inicializar el pin correspondiente al Push-Button S2 (P1.1)
	P1DIR &= ~0x02;			// Definir como entrada
	P1REN |= 0x02;			// Habilitar resistencia Pull-Up/Down
	P1OUT |= 0x02;			// Seleccionar resistencia Pull-Up

	//------- Inicializar interrupciones para pin de S1 (P2.1)
	P2IES |= 0x02; 			// Interrupt Edge Select (Falling Edge)
	P2IFG &= ~0x02; 		// Interrupt Flag - Reset
	P2IE  |= 0x02;			// Interrupt Enable - Activado

	//------- Inicializar interrupciones para pin de S2 (P1.1)
	P1IES |= 0x02; 			// Interrupt Edge Select (Falling Edge)
	P1IFG &= ~0x02; 		// Interrupt Flag - Reset
	P1IE  |= 0x02;			// Interrupt Enable - Activado

	//------- Inicializacion de TimerA0 para generacio de PWM
	TA0CCR0  |= PWM_PERIOD_CCR - 1;	// Contador de comparador hasta 200 (periodo PWM) (8MHz/200 = 40 kHz)
	TA0CCTL1 |= OUTMOD_7;			// Configuración de salida R/S para PWM
	TA0CCR1  = PWM_PERIOD_CCR / 2;	// Configuracion para ciclo de trabajo (tiempo en alto) (inicial 50%)
	TA0CTL   |= TASSEL_2;			// Seleccion de reloj para TimerA (usar reloj interno SMCLK)
	TA0CTL   |= MC_1;				// Conteo ascendente (por defecto para PWM)

	pwmDutyPeriod = PWM_PERIOD_CCR / 2; //PWM al 50%
	newPWM = 1;

	//------ Bit de habilitacion global de interrupciones
	_BIS_SR(GIE);			//Bit-Set Status Register (General Interrupt Enable)

	while(1){
		if(newPWM){
			TA0CCR1 = pwmDutyPeriod;
			newPWM = 0;
		}
	}
	
	return 0;
}

// -------------- Cuando S1 es presionado
#pragma vector = PORT2_VECTOR 		//Vector de interrupcion de Puerto 2
__interrupt void P2_INT(void){
	if(P2IFG & 0x02){ 				//Si la bandera de P2.1 se encuentra activa, P2.1 caus'o la interrupcion
		if(pwmDutyPeriod < (PWM_PERIOD_CCR - PWM_MODIFY_STEP)){
			pwmDutyPeriod += PWM_MODIFY_STEP; //Aumentar el Duty Cycle de PWM
		}
		newPWM = 1;					//Levantar bandera para indicar que hay cambio de dutyCycle
		P2IFG &= ~0x02; 			//Obligatorio bajar (estado logico 0) la bandera correspondiente
	}
}


// -------------- Cuando S2 es presionado
#pragma vector = PORT1_VECTOR 		//Vector de interrupcion de Puerto 2
__interrupt void P1_INT(void){
	if(P1IFG & 0x02){ 				//Si la bandera de P2.1 se encuentra activa, P2.1 caus'o la interrupcion
		if(pwmDutyPeriod > PWM_MODIFY_STEP){
			pwmDutyPeriod -= PWM_MODIFY_STEP; //Disminuir el Duty Cycle de PWM
		}
		newPWM = 1;					//Levantar bandera para indicar que hay cambio de dutyCycle
		P1IFG &= ~0x02; 			//Obligatorio bajar (estado logico 0) la bandera correspondiente
	}
}
