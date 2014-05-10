#include "FSL/FSL.h"

// Sensor variable
Sensor sensor;

void init_sensor(void){
	uint8 i;
	for (i = 0; i < 8; i++) {
		sensor.index[i] = 4095;
	}
	sensor.treshold = 500;
	sensor.error=0;
	sensor.lastDirection = middle;
	sensor.maxBlackCount = 5;
	sensor.minBlackCount = 2;
	sensor.isFound = 0;
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
	uint8 numberOfBlackFound=0;
	int16 indexFirstBlack=-1;
	int16 indexLastBlack=-1;
	
	//Copie des donnees, binarization, denombration du nombre de noir
	for(i=0; i<8; i++){
		sensor.index[i] = irSensor[i];
		if(sensor.index[i] > sensor.treshold){
			sensor.binarizedIndex[i] = 1;
			numberOfBlackFound++;
			if(indexFirstBlack == -1){ //Si un noir n'a encore pas ete trouvé, c'est le premier.
				indexFirstBlack = i;
			}
			indexLastBlack = i; //Le tableau est parcourus dans de 0 à 7, le dernier element qui satifera cette condition est donc le dernier noir
		}
		else{
			sensor.binarizedIndex[i] = 0;
		}
	}
	
	//Analyse des infos obtenu
	//Si aucune bande noir n'a ete trouvee
	if(indexFirstBlack == -1 || indexLastBlack == -1){
		sensor.isFound = 0;
		//ihm_led(-1, 0, 0, 0);
	}
	//Si la largeur de la bande ne correpond pas auc standards définis
	else if(numberOfBlackFound > sensor.maxBlackCount || numberOfBlackFound < sensor.minBlackCount){
		sensor.isFound = 0;
		//ihm_led(-1, 0, 0, 0);
	}
	//Sinon, la bande est condiérée comme valide
	else{
		//Cacul de la position du centre
		sensor.error = (indexLastBlack*10 + indexFirstBlack*10)/2 - 35;
		
		//Memoristaion de la direction
		if(sensor.error < -15){
			sensor.lastDirection = right;
		}
		else if(sensor.error >= -15 && sensor.error <= 15){
			sensor.lastDirection = middle;
		}
		else if(sensor.error > 15){
			sensor.lastDirection = left;
		}
		
		//Passage d'etat de ligne à trouvé
		sensor.isFound = 1;
		//ihm_led(1, 0, 0, 0);
	}
	
}

void isSensorLedTest(void){
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
