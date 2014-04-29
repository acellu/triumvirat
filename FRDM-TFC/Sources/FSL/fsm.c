
#include "TFC/TFC.h"
#include "FSL/FSL.h"

//#define NO_MOTOR

extern Line line;
extern Sensor sensor;

/*
 * Fonction d'initialisation
 */
void init_fsm(void){
	
	/* Initialise the line variable */
	init_line();
	
	/* Initialise correctors */
	init_correctors();
	
	/* Set the Default duty cycle to 0 % duty cycle */
	MOTOR_STOP;
	TFC_HBRIDGE_DISABLE;
	
	/* Centering servo channel 0 and 1 */
	SERVO_INIT;
	
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
				
			/* ------------- SURPRISE -------------- */
				
				if (TFC_DIP_SWITCH3) {
					start_competition();
				}
			/* ------------------------------------- */
				
				LED_GO_ACCEPTED;
				
				/* Enable HBRIDGE useful for motor */
				TFC_HBRIDGE_ENABLE;
				
				line.scan_number = 0;
				
				TFC_Delay_mS(1000);
				
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
			MOTOR_STOP;
			TFC_HBRIDGE_DISABLE;
			
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


