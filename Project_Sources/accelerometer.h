/*
 * accelerometer.h
 *
 *  Created on: Jan 23, 2014
 *      Author: Axel
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "common.h"

typedef struct {
	uint8 accelero_x;
	uint8 accelero_y;
	uint8 accelero_z;
}CoordAccelerometer;

//Function declarations
void init_acelerometer(void);
void accelerometer_update(void);
signed char convert (signed char input);
void time_delay_ms(unsigned int count_val);

#endif /* ACCELEROMETER_H_ */
