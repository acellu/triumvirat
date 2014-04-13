/*
 * FSL.h
 *
 *  Created on: Apr 13, 2014
 *      Author: Axel
 */

#ifndef FSL_H_
#define FSL_H_

#include "FSL/ihm.h"
#include "FSL/camera.h"
#include "FSL/fsm_main.h"
#include "FSL/correctors.h"
#include "FSL/signalProcessing.h"

void FSL_Init();

void setMotorPWM(float MotorA , float MotorB);

void setServoAngle(float angle);

#endif /* FSL_H_ */
