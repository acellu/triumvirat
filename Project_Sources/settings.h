#ifndef SETTINGS_H_
#define SETTINGS_H_

/**
 * Header regroupant les principaux paramètres utilisés par les correcteurs
 * (vitesse et angle des roues)
 * Vitesse : Correcteur proportionnel (P)
 * Angle des roues : Correcteur proportionnel intégrale (PI)
 */


#define SPEED_DUTY_MAX (float)60
#define SPEED_DUTY_MIN (float)30
#define SPEED_OFFSET_HIGH (float)10
#define SPEED_OFFSET_LOW  (float)10

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
#define ANGLE_PROPORTIONNAL_FACTOR (float)0.15
#define ANGLE_INTEGRATOR_FACTOR (float)0.1
#define ANGLE_SUM_FACTOR (float)0.02
/// Advice : maximum order wanted / ANGLE_INTEGRATOR_FACTOR, can be above the possible order of the servo-motor, it will create a latency to react
#define ANGLE_INTEGRATOR_SUM_MAX 500
#define ANGLE_INTEGRATOR_SUM_MIN -500

#define ANGLE_SAMPLE_TIME 0x15924
//#define ANGLE_SAMPLE_TIME 0x1D924
//#define ANGLE_SAMPLE_TIME 0x9D924

/*
 * Trajectories manager
 */
/// Number of acquisition keep for trajectories computing
#define TRAJECTORIES_NUMBER_ACQUISITION 40

#endif /* SETTINGS_H_ */
