/*
 * ihm.h
 *
 *  Created on: Apr 13, 2014
 *      Author: Axel
 */

#ifndef IHM_H_
#define IHM_H_

#include "TFC/TFC_arm_cm0.h"

#define LED_PROGRAM_RUNNING			ihm_led(1,0,0,0)
#define LED_GO_ACCEPTED				ihm_led(0,1,0,0)
#define LED_FOLLOWING_LINE			ihm_led(0,0,1,0)
#define LED_FINISH_LINE_DETECTED	ihm_led(0,0,0,1)
#define LED_CLEAR_ALL				ihm_led(-1,-1,-1,-1);

void init_ihm ();

void ihm_led (int8 led0, int8 led1, int8 led2, int8 led3);

uint8 ihm_switch(void);

#endif /* IHM_H_ */
