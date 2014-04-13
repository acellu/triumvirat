
#include "TFC/TFC.h"
#include "FSL/FSL.h"


extern Line line;

/*
 * Fonction d'initialisation
 */
void init_fsm(void){
	/* Initialise the line variable */
	init_line();
	
	/* Set the Default duty cycle to 0 % duty cycle */
	setMotorPWM(0,0);
	
	/* Centering servo channel 0 and 1 */
	setServoAngle(0);
	
	/* Clear all led */
	LED_CLEAR_ALL;
	
	/* Program Running */
	LED_PROGRAM_RUNNING;
}

/*
 * FSM générale
 */
void fsm(void){

	static etat_e etat = Init; //Variable de la FSM. Choix de l etat de départ de la FSM.
	
	switch(etat){
	
		case Init :
			init_fsm(); //Initialise les variables pour la FSM
			etat = Wait_start; //Etat suivant
		break;
		
		case Wait_start :
			/* Boutton Start */
			if(TFC_PUSH_BUTTON_0_PRESSED){  //If pressed...
				LED_GO_ACCEPTED;
				line.scan_number = 0;
				/*
				time_delay_ms(1500); //1,5 seconde
				*/
				etat = Following_line;
			}
			/*Boutton Reset */
			if(TFC_PUSH_BUTTON_1_PRESSED) //If pressed...
				etat = Init;
		break;
		
		case Following_line :
			//Computing only if it's a new sample (cf ANGLE_SAMPLE_TIME definition) and we know the error (line position)
			
			if(PIT_TFLG1 == 1){
				//Clear the flag
				PIT_TFLG1 = 1;
				angle_manager();
			}
			
			#ifndef NO_MOTOR
			speed_manager();
			#endif
			#ifdef NO_MOTOR
			#warning Motors are disabled
			#endif
			if((line.isFinishLine == 1) && (line.scan_number > DEFAULT_NUMBER_OF_LINE))
				etat = Stop;
			/*Boutton Reset */
			if(TFC_PUSH_BUTTON_1_PRESSED) //If pressed...
				etat = Init;
		break;
		
		case Stop :
			/* Stop Motors */
			TFC_SetMotorPWM(-1.0,-1.0);
			
			//Computing only if it's a new sample (cf ANGLE_SAMPLE_TIME definition)
			
			if(PIT_TFLG1 == 1){
				//Clear the flag
				PIT_TFLG1 = 1;
				angle_manager();
			}
			
			/*Boutton Reset */
			if(TFC_PUSH_BUTTON_1_PRESSED) //If pressed...
				etat = Init;
		break;
		
		default :
			etat = Init;
		break;		
	}
}


