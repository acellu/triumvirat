#include "correctors.h"
#include "signalProcessing.h"
#include "common.h"
#include "pwm.h"

/// // Global variables
// Position of the middle of the line (255 = not found)
extern Line line;
// Speed corrector variables
float speedCoefLow=(SPEED_DUTY_MAX - SPEED_DUTY_MIN)/(64 - SPEED_OFFSET_LOW);
float speedCoefHigh=(SPEED_DUTY_MAX - SPEED_DUTY_MIN)/(64 - SPEED_OFFSET_HIGH);
float speedDutyLeft;
float speedDutyRight;
// Angle corrector variables
long double angle_sum=0;
long double angle_order;
// Trajectories manager
//int8 trajectoriesPositionTable[TRAJECTORIES_NUMBER_ACQUISITION]={0};
//uint8 trajectoriesIndiceTable=0;
//int8 trajectoriesDelta = 0;

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




/**
 * Speed manager function (compute the duty cycle of the motors
 * Correcteur proportionnel
 */
void speed_manager(void){
	if(line.found == 1){
		//Computing the duty value for a line detected on the right
		if((line.position - 64) > 0){
			speedDutyLeft  = SPEED_DUTY_MAX - ((char)line.position - 64)*speedCoefHigh;
			speedDutyRight = SPEED_DUTY_MAX - ((char)line.position - 64)*speedCoefLow;
		}
		//Computing the duty value for a line detected on the left
		else{
			speedDutyLeft  = SPEED_DUTY_MAX - (64 - (char)line.position)*speedCoefLow;
			speedDutyRight = SPEED_DUTY_MAX - (64 - (char)line.position)*speedCoefHigh;
		}
		
		//Checking if the values of not smaller than the MIN given by SPEED_DUTY_MIN
		if(speedDutyLeft < SPEED_DUTY_MIN){
			speedDutyLeft = SPEED_DUTY_MIN;
		}
		if(speedDutyRight < SPEED_DUTY_MIN){
			speedDutyRight = SPEED_DUTY_MIN;
		}
		
		//Securtié
		if(speedDutyLeft > SPEED_DUTY_MAX){
			speedDutyLeft = SPEED_DUTY_MAX;
		}
		if(speedDutyRight > SPEED_DUTY_MAX){
			speedDutyRight = SPEED_DUTY_MAX;
		}
	}
	//Checking if the line has been found by the camera
	else{
		speedDutyLeft = SPEED_NOLINE;
		speedDutyRight = SPEED_NOLINE;
	}
	
	set_left_pwm(speedDutyLeft);
	set_right_pwm(speedDutyRight);
}
/// BACKUP
/*void speed_manager(void){
	//Computing the duty value for a line detected on the right
	if((line.position - 64) > 0){
		speedDutyLeft  = SPEED_DUTY_MAX - ((char)line.position - 64)*speedCoefHigh;
		speedDutyRight = SPEED_DUTY_MAX - ((char)line.position - 64)*speedCoefLow;
	}
	//Computing the duty value for a line detected on the left
	else{
		speedDutyLeft  = SPEED_DUTY_MAX - ((char)line.position - 64)*speedCoefLow;
		speedDutyRight = SPEED_DUTY_MAX - ((char)line.position - 64)*speedCoefHigh;
	}
	
	//Checking if the values of not smaller than the MIN given by SPEED_DUTY_MIN
	if(speedDutyLeft < SPEED_DUTY_MIN){
		speedDutyLeft = SPEED_DUTY_MIN;
	}
	if(speedDutyRight < SPEED_DUTY_MIN){
		speedDutyRight = SPEED_DUTY_MIN;
	}
	//Checking if the line has been found by the camera
	if(line.found == 0){
		speedDutyLeft = 30;
		speedDutyRight = 30;
	}
	
	set_left_pwm((int)speedDutyLeft);
	set_right_pwm((int)speedDutyRight);
}*/



/**
 * Angle manager function
 * Correcteur proportionnel integral
 * 
 * \param error : the mesured error to correct using this corrector
 * 
 * /!\ Time between each call of the function is a factor of the resulting angle,
 * 		it's strongly recommended to use a timer to manage the time between each
 * 		call.
 * 
 */
void angle_corrector_v1(char error){
	//Computing
	angle_sum += ANGLE_SUM_FACTOR*(error);
	
	//Checking if it's still in the max and min defined
	if(angle_sum > ANGLE_INTEGRATOR_SUM_MAX){
		angle_sum = ANGLE_INTEGRATOR_SUM_MAX;
	}
	else if(angle_sum <  ANGLE_INTEGRATOR_SUM_MIN){
		angle_sum = ANGLE_INTEGRATOR_SUM_MIN;
	}
	
	angle_order = ANGLE_PROPORTIONNAL_FACTOR*(error) + ANGLE_INTEGRATOR_FACTOR*angle_sum; //
	
	//Limiting the angle value (physically limited)
	if((angle_order <= 35) && (angle_order >= -35)){
		set_servo_angle(angle_order);
	}
	else if(angle_order<-35){
		set_servo_angle(-35);
	}
	else if(angle_order>35){
		set_servo_angle(35);
	}
}

void angle_manager(void){
	if(line.found == 1){
		angle_corrector((char)line.position - 64);
	}
	else{
		if(line.last_direction == right){
			set_servo_angle(ANGLE_NO_LINE);
			//angle_sum = ANGLE_INTEGRATOR_SUM_MAX;
		}
		else if(line.last_direction == middle){
			set_servo_angle(0);
			//angle_sum = 0;
		}
		else{
			set_servo_angle(-ANGLE_NO_LINE);
			//angle_sum = ANGLE_INTEGRATOR_SUM_MIN;
		}
	}
}

/**
 * Trajectories manager
 * 
 *  /!\ Time between each call of the function is a factor of the resulting computed angle,
 * 		 it's strongly recommended to use a timer to manage the time between each call.
 */
/*void trajectories_manager(void){
	uint8 oldest_sample;
	//Adding the new value inside the table and finding the oldest sample
	trajectoriesIndiceTable ++;
	if(trajectoriesIndiceTable >= TRAJECTORIES_NUMBER_ACQUISITION){
		trajectoriesIndiceTable = 0;
	}
	trajectoriesPositionTable[trajectoriesIndiceTable]= line_position - 64;
	
	//Finding the oldest sample
	if(trajectoriesIndiceTable >= TRAJECTORIES_NUMBER_ACQUISITION - 1){
		oldest_sample = 0;
	}
	else{
		oldest_sample = trajectoriesIndiceTable + 1;
	}
	
	//Compute trajectories delta
	
}*/



void angle_corrector(char error){
	
	static float speedCorrector_coeff[10] = {
			ANGLE_PROPORTIONNAL_10,
			ANGLE_PROPORTIONNAL_20,
			ANGLE_PROPORTIONNAL_30,
			ANGLE_PROPORTIONNAL_40,
			ANGLE_PROPORTIONNAL_50,
			ANGLE_PROPORTIONNAL_60,
			ANGLE_PROPORTIONNAL_70,
			ANGLE_PROPORTIONNAL_80,
			ANGLE_PROPORTIONNAL_90,
			ANGLE_PROPORTIONNAL_100
	};
	
	float current_proportionnal_factor = 0;
	
	if (speedDutyLeft <= 10 ) {
		current_proportionnal_factor = speedCorrector_coeff[0];
	}else if (speedDutyLeft > 10 && speedDutyLeft <= 20) {
		current_proportionnal_factor = speedCorrector_coeff[1];
	}else if (speedDutyLeft > 20 && speedDutyLeft <= 30) {
		current_proportionnal_factor = speedCorrector_coeff[2];
	}else if (speedDutyLeft > 30 && speedDutyLeft <= 40) {
		current_proportionnal_factor = speedCorrector_coeff[3];
	}else if (speedDutyLeft > 40 && speedDutyLeft <= 50) {
		current_proportionnal_factor = speedCorrector_coeff[4];
	}else if (speedDutyLeft > 50 && speedDutyLeft <= 60) {
		current_proportionnal_factor = speedCorrector_coeff[5];
	}else if (speedDutyLeft > 60 && speedDutyLeft <= 70) {
		current_proportionnal_factor = speedCorrector_coeff[6];
	}else if (speedDutyLeft > 70 && speedDutyLeft <= 80) {
		current_proportionnal_factor = speedCorrector_coeff[7];
	}else if (speedDutyLeft > 80 && speedDutyLeft <= 90) {
		current_proportionnal_factor = speedCorrector_coeff[8];
	}else if (speedDutyLeft > 90 && speedDutyLeft <= 100) {
		current_proportionnal_factor = speedCorrector_coeff[9];
	}else {
		current_proportionnal_factor = ANGLE_PROPORTIONNAL_FACTOR;
	}
	
	//Computing
	angle_sum += ANGLE_SUM_FACTOR*(error);
	
	//Checking if it's still in the max and min defined
	if(angle_sum > ANGLE_INTEGRATOR_SUM_MAX){
		angle_sum = ANGLE_INTEGRATOR_SUM_MAX;
	}
	else if(angle_sum <  ANGLE_INTEGRATOR_SUM_MIN){
		angle_sum = ANGLE_INTEGRATOR_SUM_MIN;
	}
	
	angle_order = current_proportionnal_factor*(error);// + ANGLE_INTEGRATOR_FACTOR*angle_sum
	
	//Limiting the angle value (physically limited)
	if((angle_order <= 30) && (angle_order >= -30)){
		set_servo_angle(angle_order);
	}
	else if(angle_order < -30){
		set_servo_angle(-30);
	}
	else if(angle_order > 30){
		set_servo_angle(30);
	}
}

