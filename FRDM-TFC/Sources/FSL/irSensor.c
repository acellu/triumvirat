#include "FSL/FSL.h"

// Sensor variable
Sensor sensor;

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


void irSensorProcessing(uint16 * irSensor){
	//Copie des donnees dans la structure
	uint8 i;
	for(i=0; i<8; i++){
		sensor.index[i] = irSensor[i];
	}
	
	//LED DEBUG
	if(sensor.index[4] < sensor.treshold){
		ihm_led(1, 0, 0, 0);
	}
	else{
		ihm_led(-1, 0, 0, 0);
	}
	if(sensor.index[5] < sensor.treshold){
		ihm_led(0, 1, 0, 0);
	}
	else{
		ihm_led(0, -1, 0, 0);
	}
	if(sensor.index[6] < sensor.treshold){
		ihm_led(0, 0, 1, 0);
	}
	else{
		ihm_led(0, 0, -1, 0);
	}
	if(sensor.index[7] < sensor.treshold){
		ihm_led(0, 0, 0, 1);
	}
	else{
		ihm_led(0, 0, 0, -1);
	}
}
