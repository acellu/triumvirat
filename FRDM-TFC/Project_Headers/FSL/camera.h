/*
 * camera.h
 *
 *  Created on: Apr 30, 2014
 *      Author: Axel
 */

#ifndef CAMERA_H_
#define CAMERA_H_

//Structure event
typedef struct{
	uint8 brake;
	uint8 straight;
	uint8 finishline;
}Event;


void camera_processing(uint16 * acquisition_camera);

void speed_event(int32 * gradient);

void event_finishline(int32 * signal , int treshold);

#endif /* CAMERA_H_ */
