#include "TFC/TFC.h"
#include "FSL/FSL.h"

/*
#ifdef FINISH_LINE_IHM_YELLOWLED
#include "ihm.h"
#endif
*/

/// Variable globales
uint8 acquisition_camera[128];

/// Line variable
extern Line line;

/// line_finishCount
uint32 line_finishCount = 0;


void camera_acquisition(void) {

	//Typedef enum de la FSM (machine a etat).
	typedef enum{
		Init,
		Set_SI,
		SI_CLK,
		Reset_SI,
		Set_CLK,
		Reset_CLK,
		fin
	}etat_e;
	
	// Variable de la FSM.
	static volatile etat_e etat = Init; // Choix de l etat de départ de la FSM.
	
	static unsigned char bit; // Variable situant le bit en cours d acqusition sur les 128 envoyes par la camera. (Uint8)
	
	
		// FSM de gestion de la camera
		switch(etat){
			case Init :
			    /* Calculate the timeout value */
			    PIT_LDVAL0 = TIMER_FREQUENCY;
			    
			    /* Enable the timer */
			    PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;
			    
				// Action a realiser dans cet etat
				GPIOE_PCOR |= LOC_SI;//SI = 0
				GPIOE_PCOR |= LOC_SCLK;//clk = 0
				bit = 0;
				// Etat suivant
				etat = Set_SI;
			break;
			
			case Set_SI :
				// Action a realiser dans cet etat
				GPIOE_PSOR |= LOC_SI;//SI = 1
				GPIOE_PCOR |= LOC_SCLK;//clk = 0
				// Etat suivant
				etat = SI_CLK;
			break;
			
			case SI_CLK :
				// Action a realiser dans cet etat
				GPIOE_PSOR |= LOC_SI;//SI = 1
				GPIOE_PSOR |= LOC_SCLK;//clk = 1
				// Etat suivant
				etat = Reset_SI;
			break;
			
			case Reset_SI :
				// Action a realiser dans cet etat
				GPIOE_PCOR |= LOC_SI;//SI = 0
				GPIOE_PCOR |= LOC_SCLK;//clk = 0
				
				//Capture du premier bit
				ADC0_SC1A = ADC0_SC1A;
				
				// Etat suivant
				etat = Set_CLK;
			break;
			
			case Set_CLK :
				// ///Enregistrement du bit n
				//Si la capture est terminée
				if((ADC0_SC1A & ADC_SC1_COCO_MASK) == ADC_SC1_COCO_MASK){
					 acquisition_camera[bit] = ADC0_RA;
				}
				else{ //Sinon affichage d'une erreur (allumage du caractere % sur l'ecran lcd)
					tlcd_percent_on();
				}
								
				// Action a realiser dans cet etat
				GPIOE_PCOR |= LOC_SI;//SI = 0
				GPIOE_PSOR |= LOC_SCLK;//clk = 1

				// Etat suivant
				etat = Reset_CLK;
			break;
			
			case Reset_CLK :
				// Action a realiser dans cet etat
				GPIOE_PCOR |= LOC_SI;//SI = 0
				GPIOE_PCOR |= LOC_SCLK;//clk = 0
				
				// Etat suivant
				if (bit >= 127 ) // Si le les bits de la caméra ont été transmis 
					etat = fin;
				else{		 	 // Sinon
					//Capture du  bit n
					ADC0_SC1A = ADC0_SC1A;
					bit ++;
					etat = Set_CLK;
				}
			break;
			
			case fin :
				// Action a realiser dans cet etat
				GPIOE_PCOR |= LOC_SI;//SI = 0
				GPIOE_PCOR |= LOC_SCLK;//clk = 0
				
				/* Set the waiting time value */
				PIT_TCTRL0 &= ~PIT_TCTRL_TIE_MASK;
			    
				
				signalProcessing(acquisition_camera);

			   
			 	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK;
			 	PIT_LDVAL0 = WAITING_TIME;
			 	// Etat suivant				
			 	etat = Init;
			break;
		
		}
		
		
		
};

//void init_camera(void){
//
//	/*
//	 * Variable declaration
//	 */
//	//Variable for ADC configuration
//	tADC_Config adc_config;
//
//
//	/* 
//	 * Initialise GPIO
//	 */
//	//Configuration de PTE8 en pin libre
//	PORTE_PCR8 = PORT_PCR_MUX(1);
//	//Configuration de PTE9 en pin libre
//	PORTE_PCR9 = PORT_PCR_MUX(1);
//	//Set PTE8 et PT9 as output
//	GPIOE_PDDR |= LOC_SI;
//	GPIOE_PDDR |= LOC_SCLK;
//
//
//
//	/*
//	 * Initialize the PIT time
//	 */
//	/* Enable the clock to the PIT module */
//	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
//
//	/* Enable frozen bit for debug mode*/
//	//PIT_MCR |= PIT_MCR_FRZ_MASK;
//
//	/* Enable the PIT timer module */
//	PIT_MCR &= ~PIT_MCR_MDIS_MASK;
//
//	/* Calculate the timeout value */
//	PIT_LDVAL0 = TIMER_FREQUENCY;
//
//	/* Enable the PIT timer interrupt in the NVIC */
//	enable_irq(68);
//
//	/* Enable interrupts on PI Timer */
//	PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK;
//
//	/* Enable timer */
//	PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;
//
//	/*
//	 *  Initialise the ADC 
//	 */
//	//Disabling irq vector
//	disable_irq(ADC0_irq_no) ;
//	disable_irq(ADC1_irq_no) ; 
//
//	//Enable clock
//	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
//
//	//Configuring the structures
//	adc_config.STATUS1A = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(0b0000);
//	adc_config.STATUS1B = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(0b0000);
//	adc_config.CONFIG1 = ADLPC_NORMAL | ADIV_1 | ADLSMP_SHORT | MODE_8 | ADICLK_BUS;
//	adc_config.CONFIG2 = MUXSEL_ADCA | ADACKEN_DISABLED | ADHSC_NORMAL | ADLSTS_6;
//	adc_config.COMPARE1 = 0x0000;
//	adc_config.COMPARE2 = 0x0000;
//	adc_config.STATUS2 = ADTRG_SW | ACFE_DISABLED | ACFGT_GREATER | ACREN_DISABLED | DMAEN_DISABLED | REFSEL_EXT;
//	//adc_config.STATUS3 = CAL_OFF | ADCO_SINGLE | AVGE_DISABLED | AVGS_4;
//	adc_config.STATUS3 = CAL_OFF | ADCO_SINGLE | AVGE_ENABLED | AVGS_4;
//	adc_config.PGA = PGAEN_DISABLED | PGALP_NORMAL | PGACHP_NOCHOP | PGAG_1;
//
//	//Configuring the ADC
//	ADC_Config_Alt(ADC0_BASE_PTR, &adc_config);
//	ADC_Cal(ADC0_BASE_PTR);
//	ADC_Config_Alt(ADC0_BASE_PTR, &adc_config);
//
//}

void isr_PIT0(void){
	
	if (PIT_TFLG0 == 1){              // if a time-out has occurred
		PIT_TFLG0 = 1;                // Acquitter le flag
		PIT_CVAL0;
		camera_acquisition();
	}
	
}
