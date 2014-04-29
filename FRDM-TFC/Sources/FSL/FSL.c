/*
 * FSL.c
 *
 *  Created on: Apr 13, 2014
 *      Author: Axel
 *      
 *      Ensemble des fonctions réécrite pour adpater les 2 codes.
 */

#include "FSL/FSL.h"
#include "TFC/TFC.h"

extern Sensor sensor;

void FSL_Init(void) {

	TFC_Init();

	init_ihm();

	init_sample_time();

	init_sensor();

	init_irSensor();

}

/* ----------------------- TEST FUNCTION ------------------------ */

void test_bibiche(void){
	int i;

	ihm_blink(1500);
	LED_CLEAR_ALL;

	// 1. test moteur droit (arriere - stop - avant) [2s]
	ihm_led(1,0,0,0);
	TFC_HBRIDGE_ENABLE;
	for (i = -100; i <= 100; i = i + 10) {
		setMotorPWM(i,0);
		TFC_Delay_mS(200);
	}
	MOTOR_STOP;

	TFC_Delay_mS(1000);

	// 2. test moteur gauche (arriere - stop - avant) [2s]
	ihm_led(0,1,0,0);
	for (i = -100; i <= 100; i = i + 10) {
		setMotorPWM(0,i);
		TFC_Delay_mS(200);
	}
	MOTOR_STOP;
	TFC_HBRIDGE_DISABLE;

	TFC_Delay_mS(1000);

	// 3. test servo (gauche - milieu - droit) [2s]
	ihm_led(0,0,1,0);
	for (i = -30; i <= 30; i = i + 5) {
		setServoAngle(i);
		TFC_Delay_mS(400);
	}
	SERVO_INIT;

	ihm_led(0,0,0,1);

	TFC_Delay_mS(2000);
	LED_CLEAR_ALL;
}

void test_vitesse(void){
	ihm_blink(1000);
	TFC_HBRIDGE_ENABLE;
	MOTOR_MAX;
	TFC_Delay_mS(1000);
	MOTOR_STOP;
	TFC_HBRIDGE_DISABLE;
	LED_CLEAR_ALL;
}

void bibiche_surprise(void){
	ihm_blink(5000);
	SERVO_INIT;
	TFC_HBRIDGE_ENABLE;
	setMotorPWM(30,30);
	TFC_Delay_mS(5000);
	MOTOR_STOP;
	TFC_Delay_mS(4000);
	setMotorPWM(-35,-35);
	TFC_Delay_mS(8000);
	MOTOR_STOP;
	TFC_HBRIDGE_DISABLE;
}

void start_competition(void){
	ihm_blink(1500);
	TFC_HBRIDGE_ENABLE;
	setMotorPWM(-100,-100);
	TFC_Delay_mS(300);
	MOTOR_STOP;
	TFC_HBRIDGE_DISABLE;
	TFC_Delay_mS(3000);
	LED_CLEAR_ALL;
}

void mesure_servo(){

	int b0 = 0;
	int b1 = 0;

	if (TFC_PUSH_BUTTON_0_PRESSED) {
		b0++;
	}
	if (TFC_PUSH_BUTTON_1_PRESSED) {
		b1++;
	}

	TFC_Delay_mS(1000);	

	float val = (float)b0 / 10 + (float)b1 /100;

	TERMINAL_PRINTF("--- ANGLE 0.|X|X| ---> | %d | %d | :: | %d | ---\n\r ",b0,b1, (int)(val*100));

	TFC_SetServo(0,val);
}

/* ---------------------- CORRECTOR PARAM ----------------------- */
float getParamPot(int mul , int div){

	float pot0 = TFC_ReadPot(0);
	float pot1 = TFC_ReadPot(1);

	pot0++; //[-1.0 ; 1.0] --> [0 ; 2.0]
	pot1++;

	return pot0 / div + pot1 * mul;
}

/* ------------------------- LABVIEW ---------------------------- */

void labView(uint8 boolLinescan) {

	uint8_t i=0,j=0;

	if(TFC_Ticker[0]>100 && LineScanImageReady==1)
	{
		TFC_Ticker[0] = 0;
		LineScanImageReady=0;

		TERMINAL_PRINTF("\r\n");
		TERMINAL_PRINTF("L:");

		if (boolLinescan == 0) {
			/* affichage sous labview de 8 capteurs dans un tableau de 128 */
			for (i = 0; i < 128; i++) {
				if (i < 16 * (j+1)) {
					LineScanImage0[i] = sensor.index[j];
				}
				if (i == (16 * (j+1)) - 1) {
					j++;
				}
			}
			j = 0;
		}

		for(i=0;i<128;i++)
		{

			TERMINAL_PRINTF("%X,",LineScanImage0[i]);
		}
		for(i=0;i<128;i++)
		{
			TERMINAL_PRINTF("%X",LineScanImage1[i]);
			if(i==127)
				TERMINAL_PRINTF("\r\n",LineScanImage1[i]);
			else
				TERMINAL_PRINTF(",",LineScanImage1[i]);
		}
	}
}

/* -------------------------- MOTOR ----------------------------- */

/*
 * @param value pwm : [-100 ; 100] = [100% arriere ; 100% avant]
 * MotorA = Droit [A]
 * MotorB = Gauche [B]
 */
void setMotorPWM(float MotorD , float MotorG) {

	TFC_SetMotorPWM( MotorD / 100 , MotorG / 100 );
}


/* -------------------------- SERVO ----------------------------- */

/*
 * @param value : [-30 ; 30]
 */
void setServoAngle(float angle) {

	if(angle > SERVO_ANGLE_MAX) {
		angle = SERVO_ANGLE_MAX;
	}

	if(angle < SERVO_ANGLE_MIN) {
		angle = SERVO_ANGLE_MIN;
	}

	/* Set angle to servo 0 */
	TFC_SetServo(0, angle / 30);

	/* Set angle to servo 1 */
	//TFC_SetServo(1, angle);
}


/* ----------------------------PIT------------------------------- */

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


