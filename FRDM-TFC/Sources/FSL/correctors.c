#include "TFC/TFC.h"
#include "FSL/FSL.h"

/// // Global variables
// Position of the middle of the line (255 = not found)
extern Line line;
extern Sensor sensor;

Corrector corrector;

void init_correctors(void){
	// Speed corrector variables
	corrector.speed.coefLow = (SPEED_DUTY_MAX - SPEED_DUTY_MIN)/(64 - SPEED_OFFSET_LOW);
	corrector.speed.coefHigh = (SPEED_DUTY_MAX - SPEED_DUTY_MIN)/(64 - SPEED_OFFSET_HIGH);
	corrector.speed.dutyLeft = 0;
	corrector.speed.dutyRight = 0;

	// Angle corrector variables
	corrector.angle.proportional = ANGLE_PROPORTIONNAL_FACTOR;
	corrector.angle.integral = ANGLE_INTEGRATOR_FACTOR;
	corrector.angle.derivative = ANGLE_DERIVATOR_FACTOR;

	corrector.angle.error = 0;
	corrector.angle.last_error = 0;
	corrector.angle.integral_sum = 0;
}

/**
 * Speed manager function (compute the duty cycle of the motors
 * Correcteur proportionnel
 */
void speed_manager(void){
	if(line.found == 1){

		if((line.position - 64) > 0){
			//Computing the duty value for a line detected on the right
			corrector.speed.dutyLeft  = SPEED_DUTY_MAX - (int8)(line.position - 64) * corrector.speed.coefHigh;
			corrector.speed.dutyRight = SPEED_DUTY_MAX - (int8)(line.position - 64) * corrector.speed.coefLow;
		}else{
			//Computing the duty value for a line detected on the left
			corrector.speed.dutyLeft = SPEED_DUTY_MAX - (int8)(64 - line.position) * corrector.speed.coefLow;
			corrector.speed.dutyRight = SPEED_DUTY_MAX - (int8)(64 -line.position) * corrector.speed.coefHigh;
		}
	}else{
		//Checking if the line has been found by the camera
		corrector.speed.dutyLeft = SPEED_NO_LINE;
		corrector.speed.dutyRight = SPEED_NO_LINE;
	}

	/* set the pwm for Motors */
	setMotorPWM(corrector.speed.dutyRight, corrector.speed.dutyLeft); //security is enable
}

/**
 * Angle manager function
 * Correcteur proportionnel integral
 * 
 * \param error : the mesured error to correct using this corrector
 * 
 * @consigne : valeur de l'angle
 * @mesure : l'angle
 * @commande : PWM pour obtenir l'angle
 * 
 * /!\ Time between each call of the function is a factor of the resulting angle,
 * 		it's strongly recommended to use a timer to manage the time between each
 * 		call.
 * 
 */

void angle_manager(void){
	if(sensor.isFound == 1){
		corrector.angle.error = -sensor.error;
		angle_corrector(); 
	}

	else{
		if(sensor.lastDirection == right){
			setServoAngle(ANGLE_NO_LINE);
		}
		else if(sensor.lastDirection == middle){
			setServoAngle(SERVO_ANGLE_INIT);
		}
		else{
			setServoAngle(-ANGLE_NO_LINE);
		}
	}
}

void angle_corrector(){

	float angle_order = 0;

	//setProportionalWithPWM();
	angle_order += corrector.angle.proportional * corrector.angle.error;

	if (CORRECTOR_INTEGRAL) {
		corrector.angle.integral_sum += ANGLE_SUM_FACTOR * corrector.angle.error;

		//Checking if it's still in the max and min defined
		if(corrector.angle.integral_sum > ANGLE_INTEGRATOR_SUM_MAX){
			corrector.angle.integral_sum = ANGLE_INTEGRATOR_SUM_MAX;
		}
		else if(corrector.angle.integral_sum <  ANGLE_INTEGRATOR_SUM_MIN){
			corrector.angle.integral_sum = ANGLE_INTEGRATOR_SUM_MIN;
		}

		angle_order += corrector.angle.integral * corrector.angle.integral_sum;
	}

	if (CORRECTOR_DERIVATIVE) {
		angle_order += corrector.angle.derivative * (corrector.angle.error - corrector.angle.last_error);

		//Remember the last position (error)
		corrector.angle.last_error = corrector.angle.error;
	}

	setServoAngle(angle_order);
}

void setProportionalWithPWM(void){

	if (corrector.speed.dutyLeft <= 10 ) {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_10;
	}else if (corrector.speed.dutyLeft > 10 && corrector.speed.dutyLeft <= 20) {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_20;
	}else if (corrector.speed.dutyLeft > 20 && corrector.speed.dutyLeft <= 30) {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_30;
	}else if (corrector.speed.dutyLeft > 30 && corrector.speed.dutyLeft <= 40) {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_40;
	}else if (corrector.speed.dutyLeft > 40 && corrector.speed.dutyLeft <= 50) {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_50;
	}else if (corrector.speed.dutyLeft > 50 && corrector.speed.dutyLeft <= 60) {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_60;
	}else if (corrector.speed.dutyLeft > 60 && corrector.speed.dutyLeft <= 70) {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_70;
	}else if (corrector.speed.dutyLeft > 70 && corrector.speed.dutyLeft <= 80) {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_80;
	}else if (corrector.speed.dutyLeft > 80 && corrector.speed.dutyLeft <= 90) {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_90;
	}else if (corrector.speed.dutyLeft > 90 && corrector.speed.dutyLeft <= 100) {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_100;
	}else {
		corrector.angle.proportional = ANGLE_PROPORTIONNAL_FACTOR;
	}
}

