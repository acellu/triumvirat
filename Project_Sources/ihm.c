#include "ihm.h"
#include "derivative.h"


void init_ihm(void){
	/*
	 * Configuring the GPIO of the leds as output, and off to defalut
	 */
	//Multiplexing
	PORTC_PCR7  = PORT_PCR_MUX(1)  | PORT_PCR_DSE_MASK;
	PORTC_PCR8  = PORT_PCR_MUX(1)  | PORT_PCR_DSE_MASK;
	PORTC_PCR9  = PORT_PCR_MUX(1)  | PORT_PCR_DSE_MASK;
	PORTC_PCR11  = PORT_PCR_MUX(1)  | PORT_PCR_DSE_MASK;
	//Direction register
	GPIOC_PDDR |= LOC_LED_BLUE;
	GPIOC_PDDR |= LOC_LED_YELLOW;
	GPIOC_PDDR |= LOC_LED_GREEN;
	GPIOC_PDDR |= LOC_LED_ORANGE;
	//Setting all leds to off
	GPIOC_PSOR |= LOC_LED_BLUE;
	GPIOC_PSOR |= LOC_LED_YELLOW;
	GPIOC_PSOR |= LOC_LED_GREEN;
	GPIOC_PSOR |= LOC_LED_ORANGE; 
	
	/*
	 * Configuring the GPIO for the push buttons, with all interruption configured, but disabled
	 */
	//Multiplexing
	PORTC_PCR5  = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;
	PORTC_PCR13  = PORT_PCR_MUX(1)  | PORT_PCR_DSE_MASK;
	//Setting pull up
	PORTC_PCR5 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;
	PORTC_PCR13 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;
	//Direction register
	GPIOC_PDDR &= ~LOC_SW3;
	GPIOC_PDDR &= ~LOC_SW4;
	//Setting the interruption (NVIC) for the port C
	enable_irq(89);
}



void ihm_sw3_it_enable(void){
	PORTC_PCR5 |= PORT_PCR_IRQC(0b1001);
}

void ihm_sw3_it_disable(void){
	PORTC_PCR5 |= PORT_PCR_IRQC(0b0000);
}

void ihm_sw4_it_enable(void){
	PORTC_PCR13 |= PORT_PCR_IRQC(0b1001);
}

void ihm_sw4_it_disable(void){
	PORTC_PCR13 |= PORT_PCR_IRQC(0b0000);
}

void ihm_led_off(LED led){
	switch(led){
		case Orange :
			GPIOC_PSOR |= LOC_LED_ORANGE;
			break;
		case Yellow :
			GPIOC_PSOR |= LOC_LED_YELLOW;
			break;
		case Blue :
			GPIOC_PSOR |= LOC_LED_BLUE;
			break;
		case Green :
			GPIOC_PSOR |= LOC_LED_GREEN;
			break;
	}
}

void ihm_led_on(LED led){
	switch(led){
			case Orange :
				GPIOC_PCOR |= LOC_LED_ORANGE;
				break;
			case Yellow :
				GPIOC_PCOR |= LOC_LED_YELLOW;
				break;
			case Blue :
				GPIOC_PCOR |= LOC_LED_BLUE;
				break;
			case Green :
				GPIOC_PCOR |= LOC_LED_GREEN;
				break;
	}
}
