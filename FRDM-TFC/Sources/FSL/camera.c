/*
 * camera.c
 *
 *  Created on: Apr 30, 2014
 *      Author: Axel
 */

#include "TFC/TFC.h"
#include "FSL/FSL.h"

Event event;

extern Line line;

void camera_processing(uint16 * acquisition_camera){

	int32 gradient[128];
	int32 treated_gradient[128];

	gradient_compute(acquisition_camera, gradient);

	gradient_moyenneMobile(gradient, treated_gradient);

	gradient_moyenneMobile3(treated_gradient, gradient);

	speed_event(gradient);

}

void speed_event(int32 * gradient){
	uint8 i;
	// Data collected for analysis (min max position and value), inside the scanned pixels
	LineData data;	

	//Initialisation de data
	data.maxValue = 500;
	data.minValue = -500;

	//Recherche de maximum et de minimum (position et valeur), ainsi que le referencement de tout les pics
	for (i=FIRST_SCANNED_VALUE; i<LAST_SCANNED_VALUE; i++) {
		if(gradient[i] > data.maxValue){
			data.maxValue = gradient[i];
			data.maxPosition = i;
		}
		else if(gradient[i] < data.minValue){
			data.minValue = gradient[i];
			data.minPosition = i;
		}
	}


	//Checking if it's a possible line (value of the max)
	if((data.maxValue < GRADIENT_MINIMUM_VALUE) || (data.minValue > -GRADIENT_MINIMUM_VALUE)){
		line.found = 0;
	}
	//If the line is too close of the edge of the camera (sensible to noise) => using a trigger
	// => s'il avait perdu la ligne a droite, il faut que la position du max soit superieur à l'offset + premiere valeur scannée pour qu'il la retrouve
	else if((line.found == 0) && (data.maxPosition < (EDGE_NOISE_OFFSET + FIRST_SCANNED_VALUE)) && line.last_direction == left){
		line.found = 0;
	}
	//If the line is too close of the edge of the camera (sensible to noise) => using a trigger
	// => s'il avait perdu la ligne a gauche, il faut que la position du min soit superieur à derniere valeur scannée - l'offset pour qu'il la retrouve
	else if((line.found == 0) && (data.minPosition > (LAST_SCANNED_VALUE - EDGE_NOISE_OFFSET)) && line.last_direction == right){
		line.found = 0;
	}
	//Checking if it's a possible line (width)
	else if(((data.minPosition - data.maxPosition) > MAXIMUM_WIDTH) || 
			((data.minPosition - data.maxPosition) < MINIMUM_WIDTH)){
		line.found = 0;
	}
	else{
		//It's not a finish line, so we switch off the led that indicate it and set isFinihLine to 0
		//ihm_led(0,0,0,-1);

		line.isFinishLine = 0;
		event.finishline = FALSE;

		line.found = 1;

		line.width = data.minPosition - data.maxPosition;

		//Checking if the deplacement of the line was not too fast
		if( ((((data.minPosition + data.maxPosition) / 2) - line.position) < MAXIMUM_LINE_MOVEMENT_ALLOWED) &&
				((((data.minPosition + data.maxPosition) / 2) - line.position) > -MAXIMUM_LINE_MOVEMENT_ALLOWED)	){
			line.position = ((data.minPosition + data.maxPosition) / 2);
		}

		/* ------------------ EVENT ------------------ */

		if(line.position > THRESHOLD_RIGHT){

			line.last_direction = right;
			event.brake = TRUE;
			event.straight = FALSE;
			ihm_led(0,1,-1,-1);

		}else if (line.position < THRESHOLD_LEFT) {

			line.last_direction = left;
			event.brake = TRUE;
			event.straight = FALSE;
			ihm_led(0,1,-1,-1);

		}else{

			line.last_direction = middle;
			event.brake = FALSE;
			event.straight = TRUE;
			ihm_led(0,-1,1,-1);
		}
		/* ------------------------------------------- */
	}
	
	//event_finishline(gradient , data.maxValue);

	line.scan_number++;

	line.linedata = data;
}

void event_finishline(int32 * signal , int treshold){
	
	uint8 i;
	int8 numberPeak = 0;


	for (i=FIRST_SCANNED_VALUE; i<LAST_SCANNED_VALUE; i++) {
		if(signal[i] > treshold){
			numberPeak++;
		}
		else if(signal[i] < -treshold){
			numberPeak++;
		}
	}
	
	//Attention croisement
	if (numberPeak >= 4 && numberPeak <= 6) {
		event.finishline = TRUE;
		event.brake = FALSE;
		event.straight = FALSE;
		ihm_led(0,-1,-1,1);
	}
}
