#include "fsm_main.h"
#include "common.h"
#include "ihm.h"
#include "correctors.h"
#include "accelerometer.h"
#include "signalProcessing.h"

extern Line line;

/*
 * Fonction d'initialisation
 */
void init_fsm(void){
	/* Initialise the line variable */
	init_line();
	set_left_pwm(0);
	set_right_pwm(0);
	set_servo_angle(0);
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
			ihm_led_off(Orange);
		break;
		case Wait_start :
			/* Boutton Start */
			if((GPIOC_PDIR & LOC_SW4) == 0){  //If pressed...
				ihm_led_on(Orange);
				line.scan_number = 0;
				time_delay_ms(1500); //1,5 seconde
				etat = Following_line;
			}
			/*Boutton Reset */
			if((GPIOC_PDIR & LOC_SW3) == 0) //If pressed...
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
			if((GPIOC_PDIR & LOC_SW3) == 0) //If pressed...
				etat = Init;
		break;
		case Stop :
			set_left_pwm(0);
			set_right_pwm(0);
			//Computing only if it's a new sample (cf ANGLE_SAMPLE_TIME definition)
			if(PIT_TFLG1 == 1){
				//Clear the flag
				PIT_TFLG1 = 1;
				angle_manager();
			}
			/*Boutton Reset */
			if((GPIOC_PDIR & LOC_SW3) == 0) //If pressed...
				etat = Init;
		break;
		
		default :
			etat = Init;
		break;		
	}
}


