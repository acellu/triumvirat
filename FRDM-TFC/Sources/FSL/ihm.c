/*
 * ihm.c
 *
 *  Created on: Apr 13, 2014
 *      Author: Axel
 */

#include "TFC/TFC.h"
#include "FSL/FSL.h"

void init_ihm (){

	/* Turn off all leds */
	LED_CLEAR_ALL;
}

/* 
 * Allume les leds suivant leurs positions et leurs valeurs : 
 * -1 = eteindre 
 * 0 (ou autre) = rien
 * 1 = allumer
 * 
 * Exemple : 
 * 		ihm_led(1,-1,-1,-1) -> allume la led 1 (gauche) et eteind les autres
 */
void ihm_led (int8 led0, int8 led1, int8 led2, int8 led3){

	if (led0 == -1) {
		TFC_BAT_LED0_OFF;
	} else if (led0 == 1) {
		TFC_BAT_LED0_ON;
	}

	if (led1 == -1) {
		TFC_BAT_LED1_OFF;
	} else if (led1 == 1) {
		TFC_BAT_LED1_ON;
	}

	if (led2 == -1) {
		TFC_BAT_LED2_OFF;
	} else if (led2 == 1) {
		TFC_BAT_LED2_ON;
	}

	if (led3 == -1) {
		TFC_BAT_LED3_OFF;
	} else if (led3 == 1) {
		TFC_BAT_LED3_ON;
	}

}

/*
 * @param : void
 * @return : decimal value [0 ; 15]
 * 
 * Example : 1 1 1 0 --> val = 14
 */

uint8 ihm_switch(void){

	uint8 dec = 0;

	if (TFC_DIP_SWITCH0) {
		dec += 1;
	}

	if (TFC_DIP_SWITCH1) {
		dec += 2;
	}

	if (TFC_DIP_SWITCH2) {
		dec += 4;
	}

	if (TFC_DIP_SWITCH3) {
		dec += 8;
	}

	return dec;
}


/*
 * 
 * 
 * Red: PTB18
 * Green: PTB19
 * Blue: PTD1
 * 
 * 
 */
