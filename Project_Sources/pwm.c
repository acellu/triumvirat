/*
 * pwm.c
 *
 *  Created on: Oct 22, 2013
 *      Author: Acellu
 */

#include "pwm.h"
#include "derivative.h"



/***********************************************************************/
/*************************** PWM MOTEUR ********************************/
/***********************************************************************/
void init_motor_pwm(void){
	//Enable the Clock to the FTM0 Module
	SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

	/*
	 * GPIO
	 */
	//Configuring PTD4 as channel 0 of FTM0 output
	PORTD_PCR4  = PORT_PCR_MUX(4)  | PORT_PCR_DSE_MASK;
	//Configuring PTD5 as channel 2 of FTM0 output
	PORTD_PCR5  = PORT_PCR_MUX(4)  | PORT_PCR_DSE_MASK;
	//Set PTD4 and PTD5 as output
	GPIOD_PDDR |= LOC_LEFT_PWM;
	GPIOD_PDDR |= LOC_RIGHT_PWM;
	//Disable the motor
	disable_right_motor();
	disable_left_motor();
	
	/*
	 * Configuration of the FTM
	 */
	//Disable write protection
	FTM0_MODE |= FTM_MODE_WPDIS_MASK;

	//Disable FTM
	FTM0_MODE &= ~1;
	//Disable PWMSYNCRO
	FTM0_MODE &= ~FTM_MODE_PWMSYNC_SHIFT;

	//Set Edge Aligned PWM in the configuration register (disable QUADEN, disable combine/Decapen)
	FTM0_QDCTRL &=~FTM_QDCTRL_QUADEN_MASK;
	FTM0_COMBINE = 0x00000000;

	//Set the modulo value for obtening an 10kHz PWM frequency (Oscilloscope : 10.35kHz)
	FTM0_CNT = 0x0;
	FTM0_MOD = PWM_MOD_VALUE;
	FTM0_CNTIN = 0;
	
	//Re-enable write protection
	FTM0_MODE &= ~FTM_MODE_WPDIS_MASK;
	
	/*
	 * Configuration of the channels
	 */
	//Configuring the Channel 4 for Edge aligned PWM (duty_cycle = 0 at beginning)
	FTM0_C4SC |= FTM_CnSC_ELSB_MASK;
	FTM0_C4SC &= ~FTM_CnSC_ELSA_MASK;
	FTM0_C4SC |= FTM_CnSC_MSB_MASK;
	FTM0_C4V = PWM_MOD_VALUE/2;
	
	//Configuring the Channel 5 for Edge aligned PWM (duty_cycle = 0 at beginning)
	FTM0_C5SC |= FTM_CnSC_ELSB_MASK;
	FTM0_C5SC &= ~FTM_CnSC_ELSA_MASK;
	FTM0_C5SC |= FTM_CnSC_MSB_MASK;
	FTM0_C5V = PWM_MOD_VALUE/2;
	
	//Reset FTM0 control
	FTM0_SC = 0x00;
	//Setting prescaler
	FTM0_SC |= MOTOR_PRESC_VALUE;
	//Enable clock and prescaler (1)
	FTM0_SC |=FTM_SC_CLKS(1);
	
	
	/*
	 * Initialisation of the enable pin for the H-bridge of the left (PTE24) or right motor (PTE25)
	 * 				GPIO + Configuration + Initial value
	 */
	PORTE_PCR24 = PORT_PCR_MUX(1)  | PORT_PCR_DSE_MASK;
	PORTE_PCR25 = PORT_PCR_MUX(1)  | PORT_PCR_DSE_MASK;
	GPIOE_PDDR |= LOC_LEFT_ENABLE;
	GPIOE_PDDR |= LOC_RIGHT_ENABLE;
	GPIOE_PCOR |= LOC_LEFT_ENABLE;
	GPIOE_PCOR |= LOC_RIGHT_ENABLE;
}


void set_left_pwm(float duty_cycle){
	FTM0_C4V = (int)(((float)PWM_MOD_VALUE + 1)*(duty_cycle/100));
}

void set_right_pwm(float duty_cycle){
	FTM0_C5V = (int)(((float)PWM_MOD_VALUE + 1)*(duty_cycle/100));
}

void enable_left_motor(void){
	GPIOE_PSOR |= LOC_LEFT_ENABLE;
}

void disable_left_motor(void){
	GPIOE_PCOR |= LOC_LEFT_ENABLE;
}

void enable_right_motor(void){
	GPIOE_PSOR |= LOC_RIGHT_ENABLE;
}

void disable_right_motor(void){
	GPIOE_PCOR |= LOC_RIGHT_ENABLE;
}



/***********************************************************************/
/************************* SERVO MOTEUR ********************************/
/***********************************************************************/

void init_servo_pwm(void){
	/*
	 * GPIO Configuration
	 */
	PORTA_PCR8 |= PORT_PCR_MUX(3)  | PORT_PCR_DSE_MASK;
	GPIOA_PDDR |= LOC_SERVO_PWM;
	
	/*
	 * Flex timer configuration
	 */
	//Enable the Clock to the FTM0 Module
	SIM_SCGC6 |= SIM_SCGC6_FTM1_MASK;
		
	/*
	 * Configuration of the FTM
	 */
	//Disable write protection
	FTM1_MODE |= FTM_MODE_WPDIS_MASK;

	//Disable FTM
	FTM1_MODE &= ~1;
	//Disable PWMSYNCRO
	FTM1_MODE &= ~FTM_MODE_PWMSYNC_SHIFT;

	//Set Edge Aligned PWM in the configuration register (disable QUADEN, disable combine/Decapen)
	FTM1_QDCTRL &=~FTM_QDCTRL_QUADEN_MASK;
	FTM1_COMBINE = 0x00000000;

	//Set the modulo value for obtening an 10kHz PWM frequency (Oscilloscope : 10.35kHz)
	FTM1_CNT = 0x0;
	FTM1_MOD = SERVO_MOD_VALUE;
	FTM1_CNTIN = 0;
	
	//Re-enable write protection
	FTM1_MODE &= ~FTM_MODE_WPDIS_MASK;
	
	/*
	 * Configuration of the channels
	 */
	//Configuring the Channel 4 for Edge aligned PWM (duty_cycle = 0 at beginning)
	FTM1_C0SC |= FTM_CnSC_ELSB_MASK;
	FTM1_C0SC &= ~FTM_CnSC_ELSA_MASK;
	FTM1_C0SC |= FTM_CnSC_MSB_MASK;
	FTM1_C0V = SERVO_PWM_MIDDLE;
	
	//Reset FTM1 control
	FTM1_SC = 0x00;
	//Setting prescaler
	FTM1_SC |= SERVO_PRESC_VALUE;
	//Enable clock
	FTM1_SC |= 0b00001000;
}

#ifndef VERIFY_ANGLE
void set_servo_angle(int angle){
	FTM1_C0V = SERVO_PWM_MIDDLE + angle * SERVO_PWM_1DEGREE;
}
#else
void set_servo_angle(int angle){
	if(angle>=-35 && angle<=35){
		FTM1_C0V = SERVO_PWM_MIDDLE + angle * SERVO_PWM_1DEGREE;
	}
}
#endif
