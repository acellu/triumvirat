/*
 * FSL.c
 *
 *  Created on: Apr 13, 2014
 *      Author: Axel
 */

#include "FSL/FSL.h"
#include "TFC/TFC.h"

void FSL_Init() {
	
	TFC_Init();
	
	init_ihm();

	//FSL_InitSampleTime();
	//FSL_InitCorrectors();
	//...
	
}

/* -------------------------- MOTOR ----------------------------- */

/*
 * @param value : [0 ; 100]
 */
void setMotorPWM(float MotorA , float MotorB) {
	
	MotorA = (MotorA - 50) / 50; // [0 ; 100] -> [-50 ; 50] -> [-1.0 ; 1.0]
	MotorB = (MotorB - 50) / 50;
	
	TFC_SetMotorPWM(MotorA,MotorB);
}
/* -------------------------------------------------------------- */


/* -------------------------- SERVO ----------------------------- */

/*
 * @param value : [-35 ; 35]
 */
void setServoAngle(float angle) {
	
	if(angle > 35) {
		angle = 35;
	}
	
	if(angle < -35) {
		angle = -35;
	}
	
	angle = angle / 35; // [-35 ; 35] -> [-1.0 ; 1.0]
	
	/* Set angle to servo 0 */
	TFC_SetServo(0, angle);
	
	/* Set angle to servo 1 */
	TFC_SetServo(1, angle);
}
/* -------------------------------------------------------------- */

