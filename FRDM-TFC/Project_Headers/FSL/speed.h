/*
 * speed.h
 *
 *  Created on: Apr 29, 2014
 *      Author: Axel
 */

#ifndef SPEED_H_
#define SPEED_H_

/* ------------ MACRO ------------- */

#define SPEED_MAX					(float)80
#define SPEED_PROPORTIONAL_MAX		(float)0.30
#define SPEED_DERIVATIVE_MAX		(float)0.50
#define FAST						setMotorPWM(SPEED_MAX , SPEED_MAX)

#define SPEED_MIN					(float)40
#define SPEED_PROPORTIONAL_MIN		(float)0.30
#define SPEED_DERIVATIVE_MIN		(float)0.80
#define SLOW						setMotorPWM(SPEED_MIN , SPEED_MIN)

#define SPEED_ACC					(float)100				
#define SPEED_PROPORTIONAL_ACC		(float)0.1
#define SPEED_DERIVATIVE_ACC		(float)0.5
#define ACCELERATION				setMotorPWM(SPEED_ACC , SPEED_ACC)

#define SPEED_BRAKE					(float)-100
#define BRAKE						setMotorPWM(SPEED_BRAKE , SPEED_BRAKE)

/* -------------------------------- */

//Typedef enum de la FSM (machine a etat).
typedef enum{
	Follow_full = 0,
	Brake,
	Follow_slow,
	Acceleration
}state_e;


void init_speed(void);

void speed_fsm(void);


#endif /* SPEED_H_ */
