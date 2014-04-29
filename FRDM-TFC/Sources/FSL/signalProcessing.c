#include "TFC/TFC.h"
#include "FSL/FSL.h"

/*
 * Voir TFC_ADC.c
 */

// Line variable
Line line;

// Linescan variable
//Linescan linescan[2];

// Sensor variable
Sensor sensor;

void init_line(void){
	line.found = 0;
	line.isFinishLine = 0;
	line.last_direction = middle;
	line.position = 64;
	line.scan_number = 0;
	line.width = 0;
}

/*
void init_linescan(uint8 channel , uint16 * line){
	uint8 i;
	for (i = 0; i < 128; ++i) {
		linescan[channel].acquisition[i] = line[i];
	}
}
*/


void signalProcessing(uint16 * acquisition_camera){

	int32 gradient[128];
	int32 treated_gradient[128];

	gradient_compute(acquisition_camera, gradient);

	gradient_moyenneMobile(gradient, treated_gradient);

	gradient_moyenneMobile3(treated_gradient, gradient);

	gradient_computeLineData(gradient);

}

void irSensorProcessing(uint16 * irSensor){
	//Copie des donnees dans la structure
	uint8 i;
	for(i=0; i<8; i++){
		sensor.index[i] = irSensor[i];
	}
	
	//LED DEBUG
	if(sensor.index[4] < sensor.treshold){
		ihm_led(1, 0, 0, 0);
	}
	else{
		ihm_led(-1, 0, 0, 0);
	}
	if(sensor.index[5] < sensor.treshold){
		ihm_led(0, 1, 0, 0);
	}
	else{
		ihm_led(0, -1, 0, 0);
	}
	if(sensor.index[6] < sensor.treshold){
		ihm_led(0, 0, 1, 0);
	}
	else{
		ihm_led(0, 0, -1, 0);
	}
	if(sensor.index[7] < sensor.treshold){
		ihm_led(0, 0, 0, 1);
	}
	else{
		ihm_led(0, 0, 0, -1);
	}
}


void gradient_compute(uint16 * acquisition_camera, int32 * gradient){
	uint8 i = 0;

	for (i=0; i<128; i++) {
		//Calcul du gradient
		if(i < 127) {
			gradient[i] = acquisition_camera[i] - acquisition_camera[i + 1];
		}
		else {
			gradient[127] = 0; //Dernière valeur aucun changement (0)
		}
	}
}

void gradient_computeLineData(int32 * gradient){

	uint8 i;
	// Data collected for analysis (min max position and value), inside the scanned pixels
	LineData data;	
	// Number of "peak" above the GRADIENT_MINIMUM_VALUE and their coordinate
	uint8 numberofPeak=0;
	// Peak variable, used to know the location of every peak detected in the signal processing (used for finishline finding)
	Peak peak[6];

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
		ihm_led(0,0,0,-1);

		line.isFinishLine = 0;
		line.found = 1;
		line.width = data.minPosition - data.maxPosition;

		//Checking if the deplacement of the line was not too fast
		if( ((((data.minPosition + data.maxPosition) / 2) - line.position) < MAXIMUM_LINE_MOVEMENT_ALLOWED) &&
				((((data.minPosition + data.maxPosition) / 2) - line.position) > -MAXIMUM_LINE_MOVEMENT_ALLOWED)	){
			line.position = ((data.minPosition + data.maxPosition) / 2);
		}
		else{
			//void
		}

		if(line.position > THRESHOLD_RIGHT)
			line.last_direction = right;
		else if(line.position < THRESHOLD_LEFT)
			line.last_direction = left;
		else
			line.last_direction = middle;
	}


	//Finding 6 peak or less in the signal
	numberofPeak = gradient_peakDetection(gradient, peak, (data.maxValue-FINISH_LINE_MAX_OFFSET));
	//Checking if it's a finish line	
	if(numberofPeak >= 4 && numberofPeak < 7){
		//gradient_checkIfFinishLine_old(numberofPeak, peak);
	}

	line.scan_number++;

	line.linedata = data;

}

/*
 * Choix des conditions pour chaques versions (1 à 3) de la fonction gradient_checkIfFinishLine
 * Return 1 si la condition d'une version est valide sinon 0
 * Defaut : return 0
 * 
 */
int gradient_checkIfFinishLine_condition(int8 firstPeak, int8 lastPeak, Peak * peak, uint8 version){
	switch (version) {
	case 1 :
		if( ((lastPeak-firstPeak) >= 3) &&
				(peak[firstPeak].signe == negatif) &&
				(peak[firstPeak+1].signe == positif) &&
				(peak[firstPeak+2].signe == negatif) &&
				(peak[firstPeak+3].signe == positif) &&
				((peak[lastPeak].position - peak[firstPeak].position) < FINISH_LINE_MAX_WIDTH) &&
				((peak[firstPeak+1].position - peak[firstPeak].position) < FINISH_LINE_WHITE_WIDTH) &&
				((peak[firstPeak+2].position - peak[firstPeak+1].position) < FINISH_LINE_BLACK_WIDTH) &&
				((peak[firstPeak+3].position - peak[firstPeak+2].position) < FINISH_LINE_WHITE_WIDTH)
		)
		{
			return 1;
		}else{
			return 0;
		}
		break;

	case 2 :
		if(
				(
						((lastPeak - firstPeak) == 2) &&
						(peak[firstPeak].signe == negatif) &&
						(peak[firstPeak+1].signe == positif) &&
						(peak[firstPeak+2].signe == negatif) &&
						((peak[lastPeak].position - peak[firstPeak].position) < FINISH_LINE_MAX_WIDTH) &&
						((peak[firstPeak+1].position - peak[firstPeak].position) < FINISH_LINE_WHITE_WIDTH) &&
						((peak[firstPeak+2].position - peak[firstPeak+1].position) < FINISH_LINE_BLACK_WIDTH)
				)
				||
				(
						((lastPeak - firstPeak) == 2) &&
						(peak[firstPeak].signe == positif) &&
						(peak[firstPeak+1].signe == negatif) &&
						(peak[firstPeak+2].signe == positif) &&
						((peak[lastPeak].position - peak[firstPeak].position) < FINISH_LINE_MAX_WIDTH) &&
						((peak[firstPeak+1].position - peak[firstPeak].position) < FINISH_LINE_BLACK_WIDTH) &&
						((peak[firstPeak+2].position - peak[firstPeak+1].position) < FINISH_LINE_WHITE_WIDTH)
				)
		)
		{
			return 1;
		}else{
			return 0;
		}
		break;

	case 3 :
		if(
				(
						((lastPeak - firstPeak) >= 3) &&
						((peak[firstPeak].signe == negatif) || (peak[firstPeak].signe == positif)) &&
						((peak[firstPeak+1].signe == negatif) || (peak[firstPeak+1].signe == positif)) &&
						((peak[firstPeak+2].signe == negatif) || (peak[firstPeak+2].signe == positif)) &&
						((peak[firstPeak+3].signe == negatif) || (peak[firstPeak+3].signe == positif))
				)
		)
		{
			return 1;
		}else{
			return 0;
		}
		break;

	default :
		return 0;
		break;
	}
}


void gradient_checkIfFinishLine(uint8 numberofPeak, Peak * peak){
	/*	
	 * Seeking the first and the last peak of the scheme (BLACK-WHITE-BLACK-WHITE-BLACK)
	 * 
	 *  _______________________________________________________
	 * |                                                       |
	 * |	   1            2    3    4    5            6      |
	 * |	   |            |    |    |    |            |      |
	 * |       v	 		v	 v    v    v            v      |
	 * |   ooooo-------------ooooo-----ooooo-------------ooo   |
	 * |							Legend : - black / o white |
	 * |_______________________________________________________|					
	 * 					Fig. Posible peaks
	 * 			
	 * Here, we want to extract the peak 2 and 5, but there are different scenarios.
	 * 
	 *  _______________________________________________________
	 * |                                                       |
	 * |	      1            2    3    4    5                |
	 * |          |            |    |    |    |                |
	 * |          v            v    v    v    v                |
	 * |   oooooooo-------------ooooo-----ooooo-------------   |
	 * |							Legend : - black / o white |
	 * |_______________________________________________________|					
	 * 				Fig. Other detection possible
	 * 				 if the line is not centred
	 * 				 
	 * 	In this case there is no peak numer 6 and we still want peak 2 and 5.
	 *  _______________________________________________________
	 * |                                                       |
	 * |	           1    2    3    4            5           |
	 * |               |    |    |    |            |           |
	 * |               v    v    v    v            v           |
	 * |   -------------ooooo-----ooooo-------------oooooooo   |
	 * |							Legend : - black / o white |
	 * |_______________________________________________________|					
	 * 				Fig. Other detection possible
	 * 				 if the line is not centred
	 * 				 
	 * 	Here we want the peak 1 and 5. 
	 * 	
	 * 	For a transition from black to white the gradient will be negative,
	 * 	For a transition from white to black the gradient will be positive,
	 * 	so the first value we want will be the first negative peak and the last value will be the last positive peak
	 * 				 
	 */

	int8 firstPeak=-1;
	int8 lastPeak=-1;
	uint8 i=0;

	//Seeking the first peak
	while((firstPeak == -1) && (i<numberofPeak)){
		if(peak[i].signe == negatif){
			firstPeak = i;
		}
		i++;
	}

	//Seeking the last peak
	i = numberofPeak-1;
	while((lastPeak == -1) && (i>0)){
		if(peak[i].signe == positif){
			lastPeak = i;
		}
		i--;
	}


	if(firstPeak != -1 && lastPeak !=-1){
		/*
		 * Checking if it can be a finish line
		 * 
		 * - 4 peaks are found (equivalent to difference of indices equal 3)
		 * - The first peak is negative
		 * - The second peak is positive
		 * - The third peak is negative
		 * - the forth peak is positive
		 * - The width of the finish found line is inferior to FINISH_LINE_MAX_WIDTH
		 * - The width of the first white found line is inferior to FINISH_LINE_MAX_WIDTH
		 * - The width of the black found line is inferior to FINISH_LINE_MAX_WIDTH
		 * - The width of the last white found line is inferior to FINISH_LINE_MAX_WIDTH
		 * 
		 */
		if(gradient_checkIfFinishLine_condition(firstPeak, lastPeak, peak, FINISH_LINE_VERSION) == 1){
			//It's a finish line
			line.found = 1;
			line.isFinishLine = 1;
			line.position = (peak[firstPeak+2].position + peak[firstPeak+1].position)/2;

			if(line.position > THRESHOLD_RIGHT)
				line.last_direction = right;
			else if(line.position < THRESHOLD_LEFT)
				line.last_direction = left;
			else
				line.last_direction = middle;


			//Switch the yellow led on to notify us
			//ihm_led_on(Yellow);
			LED_FINISH_LINE_DETECTED;
		}
	}
}


void gradient_moyenneMobile(int32 * signal, int32 * treated_signal){
	uint8 i;
	//Moyenne mobile sur 5 éléments
	for (i=2; i<125; i++) {
		treated_signal[i] = (((signal[i-2] + signal[i-1] + signal[i] + signal[i+1] + signal[i+2])*GRADIENT_AMPLIFICATION)/5);
	}
}


void gradient_moyenneMobile3(int32 * signal, int32 * treated_signal){
	uint8 i;
	//Moyenne mobile sur 5 éléments
	for (i=1; i<126; i++) {
		treated_signal[i] = ((signal[i-1] + signal[i] + signal[i+1])/3);
	}
}

// Algo actuel : angle mort à droite de la valeur 128-FINISH_LINE_FOWARD_JUMP
uint8 gradient_peakDetection(int32 * signal, Peak * peak, uint8 threshold){

	uint8 i;

	// Count the number of peak found (max 6)
	uint8 numberofpeak=0;

	for(i=FIRST_SCANNED_VALUE; i<LAST_SCANNED_VALUE; i++){
		if((signal[i] > threshold) && (numberofpeak < 6)){
			peak[numberofpeak].position = i;
			peak[numberofpeak].signe = positif;
			numberofpeak++;
			if(i<(128-FINISH_LINE_FOWARD_JUMP)){
				i += FINISH_LINE_FOWARD_JUMP;
			}
			else{
				i=LAST_SCANNED_VALUE;
			}
		}
		else if((signal[i] < -threshold) && (numberofpeak < 6)){
			peak[numberofpeak].position = i;
			peak[numberofpeak].signe = negatif;
			numberofpeak++;
			if(i<(128-FINISH_LINE_FOWARD_JUMP)){
				i += FINISH_LINE_FOWARD_JUMP;
			}
			else{
				i=LAST_SCANNED_VALUE;
			}
		}
	}

	return numberofpeak;
}
