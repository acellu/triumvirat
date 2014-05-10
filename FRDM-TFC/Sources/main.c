#include "derivative.h" /* include peripheral declarations */
#include "TFC/TFC.h"
#include "FSL/FSL.h"

extern Corrector corrector;

/********************************************************************/
int main (void){

	FSL_Init();

	while(1)
	{	
		// TFC_Task must be called in your main loop.  This keeps certain processing happy (I.E. Serial port queue check) 
		TFC_Task();

		/* FSM */
		if(TFC_DIP_SWITCH0){
			fsm();
		} 

		/* LABVIEW */
		if (TFC_DIP_SWITCH1) {

			if (TFC_PUSH_BUTTON_0_PRESSED) {
				//void

			} else if (TFC_PUSH_BUTTON_1_PRESSED) {
				labView(1); // Les 2 linescans
			} else {
				labView(0); // LineScan1 et IR 
			}
		}

		/* REGLAGE SPEED_MAX et SPEED_BRAKE + Fonctions tests */
		if (TFC_DIP_SWITCH2) {

			if (TFC_PUSH_BUTTON_0_PRESSED) {
				//void

			} else if (TFC_PUSH_BUTTON_1_PRESSED) {
				//test_bibiche();
				//test_vitesse();
				//bibiche_surprise();
				//start_competition();
			} else {
				//mesure_servo();
			}
		}

		/* CORRECTORS PARAMETER ON PUTTY */
		if (TFC_DIP_SWITCH3) {

			if(TFC_Ticker[0]>=20)
			{
				TFC_Ticker[0] = 0; //reset the Ticker
				TFC_Delay_mS(4000);

				if (TFC_PUSH_BUTTON_0_PRESSED) {
					corrector.angle.derivative = getParamPot(20,20);
					TERMINAL_PRINTF("---> ANGLE_DERIVATOR_FACTOR : | %d | <--- (factor:1000)\n\r ",(int)(corrector.angle.derivative * 1000));

				} else if (TFC_PUSH_BUTTON_1_PRESSED) {
					corrector.angle.proportional = getParamPot(20,20);
					TERMINAL_PRINTF("---> ANGLE_POPORTIONAL_FACTOR : | %d | <--- (factor:1000)\n\r ",(int)(corrector.angle.proportional * 1000));

					/*
					corrector.angle.integral = getParamPot(20,20);
					TERMINAL_PRINTF("---> ANGLE_INTEGATOR_FACTOR : | %d | <--- (factor:1000)\n\r ",(int)(corrector.angle.integral * 1000));
					 */

				} else {
					TERMINAL_PRINTF("---> ANGLE_POPORTIONAL_FACTOR : | %d | <--- (factor:1000)\n\r ",(int)(corrector.angle.proportional * 1000));
					TERMINAL_PRINTF("---> ANGLE_DERIVATOR_FACTOR : | %d | <--- (factor:1000)\n\r ",(int)(corrector.angle.derivative * 1000));
					TERMINAL_PRINTF("---> ANGLE_INTEGATOR_FACTOR : | %d | <--- (factor:1000)\n\r ",(int)(corrector.angle.integral * 1000));
				}
			}
		}
	}

	return 0;
}
/********************************************************************/


/*
int main(void)
{
	uint32_t t,i=0;

	TFC_Init();

	for(;;)
	{	   
		//TFC_Task must be called in your main loop.  This keeps certain processing happy (I.E. Serial port queue check)
		TFC_Task();

		//This Demo program will look at the middle 2 switch to select one of 4 demo modes.
		//Let's look at the middle 2 switches
		switch((TFC_GetDIP_Switch()>>1)&0x03)
		{
		default:
		case 0 :
			fsm();
			break;

		case 1:
			LED_CLEAR_ALL;
			TFC_BAT_LED1_ON;
			//Demo mode 1 will just move the servos with the on-board potentiometers
			if(TFC_Ticker[0]>=20)
			{
				TFC_Ticker[0] = 0; //reset the Ticker
				//Every 20 mSeconds, update the Servos
				TFC_SetServo(0,TFC_ReadPot(0));
				TFC_SetServo(1,TFC_ReadPot(1));
			}
			//Let's put a pattern on the LEDs
			if(TFC_Ticker[1] >= 125)
			{
				TFC_Ticker[1] = 0;
				t++;
				if(t>4)
				{
					t=0;
				}			
				TFC_SetBatteryLED_Level(t);
			}

			TFC_SetMotorPWM(0,0); //Make sure motors are off
			TFC_HBRIDGE_DISABLE;


			break;

		case 2 :
			LED_CLEAR_ALL;
			TFC_BAT_LED2_ON;
			//Demo Mode 2 will use the Pots to make the motors move
			TFC_HBRIDGE_ENABLE;
			TFC_SetMotorPWM(TFC_ReadPot(0),TFC_ReadPot(1));

			//Let's put a pattern on the LEDs
			if(TFC_Ticker[1] >= 125)
			{
				TFC_Ticker[1] = 0;
				t++;
				if(t>4)
				{
					t=0;
				}			
				TFC_SetBatteryLED_Level(t);
			}
			break;

		case 3 :
			LED_CLEAR_ALL;
			//Demo Mode 3 will be in Freescale Garage Mode.  It will beam data from the Camera to the 
			//Labview Application

			TFC_BAT_LED3_ON;

			if(TFC_Ticker[0]>100 && LineScanImageReady==1)
			{
				TFC_Ticker[0] = 0;
				LineScanImageReady=0;
				TERMINAL_PRINTF("\r\n");
				TERMINAL_PRINTF("L:");

				if(t==0)
					t=3;
				else
					t--;

				TFC_SetBatteryLED_Level(t);

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



			break;
		}
	}

	return 0;
}
 */

