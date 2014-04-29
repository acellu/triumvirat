#ifndef CORRECTORS_H_
#define CORRECTORS_H_

#include "TFC\TFC_Types.h"

/**
 * Header regroupant les principaux paramètres utilisés par les correcteurs
 * (vitesse et angle des roues)
 * Vitesse : Correcteur proportionnel (P)
 * Angle des roues : Correcteur proportionnel intégrale (PI)
 */


#define SPEED_DUTY_MAX (float)70.0
#define SPEED_DUTY_MIN (float)40.0
#define SPEED_OFFSET_HIGH (float)50.0
#define SPEED_OFFSET_LOW  (float)40.0
#define SPEED_NO_LINE (float)30.0

/*
 * About the angle corrector (PI) :
 * """"""""""""""""""""""""""""""""
 * => the general equation of the corrector is :
 *		 commande(n) = + ANGLE_PROPORTIONNAL_FACTOR * error(n) + ANGLE_INTEGRATOR_FACTOR * SUM(error(n)*ANGLE_SUM_FACTOR)
 * 		
 * => the general factor of the integral correction is ANGLE_INTEGRATOR_FACTOR*ANGLE_SUM_FACTOR, 
 * 		with, actually ANGLE_SUM_FACTOR the reel sample time,
 * 
 * => ANGLE_SAMPLE_TIME : the value used for the timer to calculate a new correction consign (sample time),
 * 
 * => the error given is the relative position of the line (0 to 128) so we have to subtract it by 64 in order to have a
 * 		variation from -64 up to 64 (need negative values for the integral corrector.
 */


/*
 * ANGLE_PROPORTIONNAL variant selon la valeur de la PWM pour la fonction correcteur
 */
/// Coefficient avec une pwm compris entre 0 et 100.
#define ANGLE_PROPORTIONNAL_10 		0.6
#define ANGLE_PROPORTIONNAL_20 		0.56
#define ANGLE_PROPORTIONNAL_30 		0.64 //0.54
#define ANGLE_PROPORTIONNAL_40 		0.28 //
#define ANGLE_PROPORTIONNAL_50 		0.08 //0.02
#define ANGLE_PROPORTIONNAL_60 		0.01
#define ANGLE_PROPORTIONNAL_70 		0.005
#define ANGLE_PROPORTIONNAL_80 		0.15
#define ANGLE_PROPORTIONNAL_90 		0.10
#define ANGLE_PROPORTIONNAL_100 	0.05

/*
 * Oscillation Kp = 1,2
 */

/* PROPORTIONNAL PARAMETER */
#define ANGLE_PROPORTIONNAL_FACTOR (float)0.55  //0.54

/* INTEGRATOR PARAMETER */
#define ANGLE_INTEGRATOR_FACTOR (float)0.001
/// Advice : maximum order wanted / ANGLE_INTEGRATOR_FACTOR, can be above the possible order of the servo-motor, it will create a latency to react
#define ANGLE_INTEGRATOR_SUM_MAX 3500
#define ANGLE_INTEGRATOR_SUM_MIN -3500

/* DERIVATOR PARAMETER */
#define ANGLE_DERIVATOR_FACTOR (float)3.0

/* SUM PARAMETER */
#define ANGLE_SUM_FACTOR (float)0.35 //0.2

#define ANGLE_NO_LINE (float)30.0


#define ANGLE_SAMPLE_TIME 0x15924
//#define ANGLE_SAMPLE_TIME 0x1D924
//#define ANGLE_SAMPLE_TIME 0x9D924

/*
 * Trajectories manager
 */
/// Number of acquisition keep for trajectories computing
//#define TRAJECTORIES_NUMBER_ACQUISITION 40


///Enable type of corrector (TRUE OR FALSE)
/* ---------------------------------------- */
#define CORRECTOR_INTEGRAL 		FALSE
#define CORRECTOR_DERIVATIVE 	TRUE
/* ---------------------------------------- */

typedef struct{
	float coefLow;
	float coefHigh;
	float dutyLeft;
	float dutyRight;
}Speed;

typedef struct{
	float proportional;
	float integral;
	float derivative;
	
	int8 error;
	int8 last_error;
	float integral_sum;
}Angle;

typedef struct{
	Angle angle;
	Speed speed;
}Corrector;



void init_correctors(void);
void angle_corrector(void);
void angle_manager(void);
void speed_manager(void);
void setProportionalWithPWM(void);


#endif /* CORRECTORS_H_ */
