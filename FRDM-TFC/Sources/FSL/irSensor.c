#include "FSL/FSL.h"

void init_sensor(void){
	uint8 i;
	for (i = 0; i < 8; i++) {
		sensor.index[i] = 4095;
	}
	sensor.treshold = 1024;
}

void init_irSensor(void){
	/* Configuration GPIO */
	//Configuration de PTC0 en pin singled-ended de ADC (Data)
	PORTC_PCR0 = PORT_PCR_MUX(0);
	//Configuration de PTC6, PTC10 et PTC11 en pin libres (Adresse)
	PORTC_PCR6 = PORT_PCR_MUX(1);
	PORTC_PCR10 = PORT_PCR_MUX(1);
	PORTC_PCR11 = PORT_PCR_MUX(1);
	
	//Set PTC6, PTC10 et PTC11 as Output (adresse)
	GPIOC_PDDR |= CTRL_MUX_MASK;
	//Set PTC0 as Output (data)
	GPIOC_PDDR |= 0x0001;
	
	/* Scan de la première valeur */
	//Reset de l'addresse envoyee au multiplexeur
	IR_SENSOR_ADDR_RESET;
	//Envoi de l'adresse 0 au multiplexer
	IR_SENSOR_ADDR_0;
}
