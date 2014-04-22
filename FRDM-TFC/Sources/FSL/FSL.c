/*
 * FSL.c
 *
 *  Created on: Apr 13, 2014
 *      Author: Axel
 *      
 *      Ensemble des fonctions réécrite pour adpater les 2 codes.
 */

#include "FSL/FSL.h"
#include "TFC/TFC.h"

void FSL_Init() {

	TFC_Init();

	init_ihm();
	
	init_sample_time();

}

/* ---------------------- CORRECTOR PARAM ----------------------- */
float getParamPot(){
	float valueParam = 0;
	
	float pot0 = TFC_ReadPot(0);
	float pot1 = TFC_ReadPot(1);
	
	pot0++; //[-1.0 ; 1.0] --> [0 ; 2.0]
	pot1++;
	
	valueParam = pot0 + pot1;
	
}

/* ------------------------- LABVIEW ---------------------------- */

void labView(void) {
	
	uint32_t i=0;
	
	if(TFC_Ticker[0]>100 && LineScanImageReady==1)
	{
		TFC_Ticker[0] = 0;
		LineScanImageReady=0;

		TERMINAL_PRINTF("\r\n");
		TERMINAL_PRINTF("L:");

		for(i=0;i<128;i++)
		{
			TERMINAL_PRINTF("%X,",LineScanImage0[i]);
		}

		for(i=0;i<128;i++)
		{
			TERMINAL_PRINTF("%X",LineScanImage1[i]);
			if(i==127)
				TERMINAL_PRINTF("\r\n",LineScanImage1[i]);
			else
				TERMINAL_PRINTF(",",LineScanImage1[i]);
		}
	}
}
/* -------------------------------------------------------------- */

/* -------------------------- MOTOR ----------------------------- */

/*
 * @param value pwm : [-100 ; 100] = [100% arriere ; 100% avant]
 * MotorA = Gauche [B]
 * MotorB = Droit [A]
 */
void setMotorPWM(float MotorA , float MotorB) {

	MotorA /= 100.0; // [-100 ; 100] -> [-1.0 ; 1.0]
	MotorB /= 100.0;

	TFC_SetMotorPWM( MotorA , MotorB );
}
/* -------------------------------------------------------------- */


/* -------------------------- SERVO ----------------------------- */

/*
 * @param value : [-35 ; 35]
 */
void setServoAngle(float angle) {

	if(angle > SERVO_ANGLE_MAX) {
		angle = SERVO_ANGLE_MAX;
	}

	if(angle < -SERVO_ANGLE_MAX) {
		angle = -SERVO_ANGLE_MAX;
	}

	angle = angle / SERVO_ANGLE_MAX; // [-SERVO_ANGLE_MAX ; SERVO_ANGLE_MAX] -> [-1.0 ; 1.0]

	/* Set angle to servo 0 */
	TFC_SetServo(0, angle);

	/* Set angle to servo 1 */
	//TFC_SetServo(1, angle);
}
/* -------------------------------------------------------------- */

int moyTab(uint16 *tab){
	int i;
	int sum = 0;
	for (i = 0; i < 128; ++i) {
		sum += tab[i];
	}
	sum = sum / 128;

	return sum;
}

/* --------------------------------------------------------------- */

void gradient(uint16 *acquisition_camera , uint16* line){
	uint8 i = 0;

	for (i=0; i<128; i++) {
		//Calcul du gradient
		if(i < 127)
			line[i] = acquisition_camera[i] - acquisition_camera[i + 1];
		else
			line[127] = acquisition_camera[127]; //Dernière valeur aucun changement
	}
}

/* -----------------------HOUGH------------------------------------ */

void transformHough(uint16* line , uint16* echant_hough){
	uint16 i = 0;
	uint8 j = 0;

	/* recuperation des échantillons par valeurs de 0 à 4096 (12bits) */

	for (i = 0; i < 4096; ++i) {
		for (j = 0; j < 128; ++j) {
			if (line[j] == i) {
				echant_hough[i]++;
			}
		}
	}
}

/* ----------------------------PIT------------------------------- */

/**
 * Function that activate and configure the timer for the management of the sample time
 * No IRS are used (low priority), PIT1 is used for this
 */
void init_sample_time(void){
	/* Enable the clock to the PIT module */
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;

	/* Enable frozen bit for debug mode*/
	PIT_MCR |= PIT_MCR_FRZ_MASK;

	/* Enable the PIT timer module */
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;

	/* Setting the timeout value */
	PIT_LDVAL1 = ANGLE_SAMPLE_TIME;

	/* Enable timer */
	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK;
} 


