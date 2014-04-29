/*
 * FSL.h
 *
 *  Created on: Apr 13, 2014
 *      Author: Axel
 */

#ifndef FSL_H_
#define FSL_H_

#include "FSL/ihm.h"
#include "FSL/fsm.h"
#include "FSL/correctors.h"
#include "FSL/signalProcessing.h"
#include "FSL/irSensor.h"

/* ------------- SERVO --------------- */

#define SERVO_ANGLE_MAX 	(float)26.0
#define SERVO_ANGLE_MIN 	(float)-19.0
#define SERVO_ANGLE_INIT 	(float)1.8
#define SERVO_INIT 			setServoAngle( SERVO_ANGLE_INIT )

/* ------------- MOTOR --------------- */

#define MOTOR_STOP 			setMotorPWM( 0 , 0 )
#define MOTOR_MAX 			setMotorPWM( 100 , 100 )

/* ----------------------------------- */

void FSL_Init(void);

void test_vitesse(void);

void test_bibiche(void);

void start_competition(void);

void mesure_servo(void);

float getParamPot(int mul , int div);

void labView(uint8 boolLinescan);

void setMotorPWM(float MotorA , float MotorB);

void setServoAngle(float angle);

int moyTab8(uint16 *tab);

void init_sample_time(void);

#endif /* FSL_H_ */
