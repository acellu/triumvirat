
#include "TFC/TFC.h"
#include "FSL/FSL.h"

extern Line line;
extern Sensor sensor;
extern Event event;


/*
 * Fonction d'initialisation
 */
void init_fsm(void){

	/* Initialise the line variable */
	init_line();

	/* Initialise correctors */
	init_correctors();

	/* Initialise speed_fsm */
	init_speed();
	
	/* Initialise the LTPMR Timer */
	lowPowerTimer_init();

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
		if(TFC_PUSH_BUTTON_0_PRESSED){

			/* ------------- SURPRISE -------------- */
			/*
				if (TFC_DIP_SWITCH2) {
					start_competition();
				}
			 */
			/* ------------------------------------- */

			LED_GO_ACCEPTED;

			/* Enable HBRIDGE useful for motor */
			TFC_HBRIDGE_ENABLE;

			//line.scan_number = 0;

			TFC_Delay_mS(1000);

			etat = Following_line;
		}
		/*Boutton Reset */
		if(TFC_PUSH_BUTTON_1_PRESSED){
			etat = Init;
		}
		
		break;
		


	case Following_line :

		if(PIT_TFLG1 == 1){
			PIT_TFLG1 = 1; //Clear the flag
			angle_manager();
		}

		/* ------- SPEED_FSM ------- */

		speed_manager();
		//speed_fsm(); //sous-FSM --> speed.c

		/* ------------------------- */

		if (event.finishline) {
			etat = Stop;
		}

		/*Boutton Reset */
		if(TFC_PUSH_BUTTON_1_PRESSED)
			etat = Init;
		break;

	case Stop :
		/* Delay pour un arrêt parfait */
		//Delay

		/* Stop Motors */
		MOTOR_STOP;
		TFC_HBRIDGE_DISABLE;

		if(PIT_TFLG1 == 1){
			PIT_TFLG1 = 1; //Clear the flag
			angle_manager();
		}

		/*Boutton Reset */
		if(TFC_PUSH_BUTTON_1_PRESSED)
			etat = Init;
		break;

	default :
		etat = Init;
		break;		
	}
}


