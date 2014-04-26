/*
 * FSM-IR.C
 *
 *  Created on: Apr 24, 2014
 *      Author: Muu La Fraga
 */

#include "FSM-IR.h"
#include "TFC_ADC.c"

/// Variables globales
uint8 acquisition_IR[8];



void capteurIR_acquisition(void){
	
	// Variable de la FSM
	static volatile etat_IR etat = Etat1; // Choix de l etat de depart de la FSM
	
	static unsigned char numero; // Variable situant le capteur en cours d acquisition
	
	static unsigned char Ab, Bb, Cb;
	
		// Definition de la FSM
	switch(etat){
		case Selection :
			if(numero == 1 | 3 | 5 | 7)
				Ab = 1;
			else
				Ab = 0;
			
			if(numero == 2 | 3 | 6 | 7)
				Bb = 1;
			else
				Bb = 0;
			
			if(numero == 4 | 5 | 6 | 7)
				Cb = 1;
			else
				Cb = 0;
			
			//entreeADC = SortieMUX; // Brancher directement, reste a copier l entree
			
			etat = Delay_Conversion;
		break;
		
		case Delay_Conversion :
			// Maintenir echantillon cb de temps ????
			// reglage effectue dans la config de ADC
			if((ADC0_SC1A & ADC_SC1_COCO_MASK) == ADC_SC1_COCO_MASK) // Si le flag de conversion est leve
				acquisition_IR[numero] = ADC0_RA; // Copie de la sortie de ADC dans un tableau
			// Increment de la variable numero (modulo 8 (de 0 a 7))
			if (numero >= 7)
				numero = 0;
			else
				numero = numero + 1;
			etat = Selection;
		break;
		
		default :
			etat = Selection;
		break;	
	} 
}

void init_IR(void){
	
	//Variable for ADC configuration
	tADC_Config adc_config;
	
	/*
	 * Initialise le GPIO
	 */
	
	//Configuration de PTC0 en pin singled-ended de ADC
	PORTC_PCR0 = PORT_PCR_MUX(0);
	//Configuration de PTC6, PTC10 et PTC11 en pin libres
	PORTC_PCR6 = PORT_MCR_MUX(1);
	PORTC_PCR10 = PORT_MCR_MUX(1);
	PORTC_PCR11 = PORT_MCR_MUX(1);
	
	//Set PTC6, PTC10 et PTC11 as Output
	GPIOC_PDDR |= CTRL_MUX_MASK;
	//Set PTC0 as Output
	GPIOC_PDDR |= 0x0001;
	
	/*
	 * Initialise ADC0
	 */
	
	InitADC0();
}
