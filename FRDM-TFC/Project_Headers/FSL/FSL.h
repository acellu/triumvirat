/*
 * FSL.h
 *
 *  Created on: Apr 13, 2014
 *      Author: Axel
 */

#ifndef FSL_H_
#define FSL_H_

#include "FSL/ihm.h"
#include "FSL/fsm_main.h"
#include "FSL/correctors.h"
#include "FSL/signalProcessing.h"

/* ------------- SERVO --------------- */

#define SERVO_ANGLE_MAX 30
#define SERVO_ANGLE_INIT setServoAngle(0)

/* ------------- MOTOR --------------- */

#define MOTOR_STOP setMotorPWM(0 , 0)

/* ----------------------------------- */

void FSL_Init();

void labView(void);

void setMotorPWM(float MotorA , float MotorB);

void setServoAngle(float angle);

int moyTab8(uint16 *tab);

void gradient(uint16 *acquisition_camera , uint16* line);

void transformHough(uint16* line , uint16* echant_hough);

void init_sample_time(void);

#endif /* FSL_H_ */
