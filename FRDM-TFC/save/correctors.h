#ifndef CORRECTORS_H_
#define CORRECTORS_H_



/**
 * Header regroupant les principaux paramètres utilisés par les correcteurs
 * (vitesse et angle des roues)
 * Vitesse : Correcteur proportionnel (P)
 * Angle des roues : Correcteur proportionnel intégrale (PI)
 */


#define SPEED_DUTY_MAX (float)70
#define SPEED_DUTY_MIN (float)40
#define SPEED_OFFSET_HIGH (float)50
#define SPEED_OFFSET_LOW  (float)40
#define SPEED_NOLINE (float)30

/*
#define SPEED_DUTY_MAX (float)50
#define SPEED_DUTY_MIN (float)40
#define SPEED_OFFSET_HIGH (float)50
#define SPEED_OFFSET_LOW  (float)40
#define SPEED_NOLINE (float)30
*/

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
//#define ANGLE_PROPORTIONNAL_FACTOR (float)0.54
#define ANGLE_PROPORTIONNAL_FACTOR (float)0.54
#define ANGLE_INTEGRATOR_FACTOR (float)0.001
//#define ANGLE_SUM_FACTOR (float)0.2
#define ANGLE_SUM_FACTOR (float)0.35
/// Advice : maximum order wanted / ANGLE_INTEGRATOR_FACTOR, can be above the possible order of the servo-motor, it will create a latency to react
#define ANGLE_INTEGRATOR_SUM_MAX 3500
#define ANGLE_INTEGRATOR_SUM_MIN -3500

#define ANGLE_SAMPLE_TIME 0x15924
//#define ANGLE_SAMPLE_TIME 0x1D924
//#define ANGLE_SAMPLE_TIME 0x9D924

//Should be between -35 and 35
#define ANGLE_NO_LINE 27

/*
 * Trajectories manager
 */
/// Number of acquisition keep for trajectories computing
//#define TRAJECTORIES_NUMBER_ACQUISITION 40


void angle_corrector(char error);
void angle_manager(void);
void angle_corrector_v1(char error);
void speed_manager(void);
void init_sample_time(void);


#endif /* CORRECTORS_H_ */
