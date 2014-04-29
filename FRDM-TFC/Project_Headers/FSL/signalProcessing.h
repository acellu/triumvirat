#ifndef SIGNALPROCESSING_H_
#define SIGNALPROCESSING_H_

#include "TFC/TFC_arm_cm0.h"


/*******************************
 * Signal processing variables *
 *******************************/
/// Number of pixel
#define NUMBER_OF_PIXEL  128
/// Pixel value max - 12 bits
#define  PIXEL_VALUE_MAX_12_BITS 4095
/// Pixel value max - 8 bits
#define PIXEL_VALUE_MAX_8_BITS 255
/// First scanned value
#define FIRST_SCANNED_VALUE 10
/// Last scanned value
#define LAST_SCANNED_VALUE 120
/// Minimum value of the gradient to be detected as a transition
#define GRADIENT_MINIMUM_VALUE 10
/// Maximum value of the gradient
#define GRADIENT_MAXIMUM_VALUE 50
/// Gradient amplification of the signal
#define GRADIENT_AMPLIFICATION 2

/******************
 * Line detection *
 ******************/
/// Minimum width of the line to be accepted as found
#define MINIMUM_WIDTH 5 //22
/// Maximum width of the line to be accepted as found
#define MAXIMUM_WIDTH 15 //35
/// Maximum line movement allowed in pixel per image(if actual line - old line > this varaible, the echantillon will be rejected)
#define MAXIMUM_LINE_MOVEMENT_ALLOWED 80
/// Threshold for setting the line as 'left' in the direction variable
#define THRESHOLD_LEFT 	55	//53
/// Threshold for setting the line as 'right' in the direction variable
#define THRESHOLD_RIGHT 73	//75
/// Offset for exclude noise when the line is too close of the camera edge (computeLineDate_v2)
#define EDGE_NOISE_OFFSET 8

/*************************
 * Finish Line detection *
 *************************/
/// The jump done when a peak is found (in pixel)
#define FINISH_LINE_FOWARD_JUMP 9
/// The offset value under the max to detect peak
#define FINISH_LINE_MAX_OFFSET 4
/// The maximum width of the finish line
#define FINISH_LINE_MAX_WIDTH 150
/// The maximum width of the white bands
#define FINISH_LINE_WHITE_WIDTH 35
/// The maximum width of the middle black line
#define FINISH_LINE_BLACK_WIDTH 35
/// The version of function gradient_checkIfFinishLine (1 to 3)
#define FINISH_LINE_VERSION 1

/*************************
 * Sensor detection *
 *************************/
/// Value of white
#define WHITE 255
/// Value of black
#define BLACK 0
/// Treshold between black and white
#define TRESHOLD_SENSOR 128



/// Variable for the last known direction of the line
typedef enum{
	left=0,
	middle,
	right
}Direction;

///Other data on this line (used for debug and signal treatement)
typedef struct{
	uint8 minPosition; 	/// Location of the minimum
	int16 minValue;		/// Value of the minimum
	uint8 maxPosition;	/// Location of the maximum
	int16 maxValue;		/// Value of the maximum
}LineData;

/// Line variable, contain all information about the line
typedef struct{
	uint8 position; 	/// Position of the line
	uint8 found;		/// Equal 0 is the line is not found, 1 if the line is found
	uint8 width;		/// Width of the line found
	Direction last_direction;	///Last know direction of the line
	Direction next_direction;	///Next direction
	LineData linedata;	/// Use for debug purpose, linedata have all the data used to compute the other variables of the line Structure
	uint8 isFinishLine;	/// Equal 1 if the current line scanned is a finish line, if not, it will be equal to 0
	uint32 scan_number; /// Count each acquisition made by the camera from the beginning, can be used to know the up time of the car
}Line;

/// Variable that contain the sign information
typedef enum{
	negatif, 			/// For a negative integer
	positif, 			/// For a positive integer
	null				/// For a null integer
}Signe_e;

/// Peak variable, used to know the location of every peak detected in the signal processing (used for finishline finding)
typedef struct{
	uint8 position;		/// Position of the peak
	uint8 index_min;
	uint8 index_max;
	uint8 index_middle;
	uint8 width;
	Signe_e signe;		/// Sign of the peak
}Peak;


/// Gradient
typedef struct{
	Peak peak[6];
	int32 signal[128];
}Gradient;

///Pixel
typedef struct{
	uint8 pixel_min;
	uint8 pixel_max;
	uint8 pixel_value_white;
	uint8 pixel_value_black;
}Pixel;


///Linescan
typedef struct{
	//Pixel pixel;
	//vuint16* acquisition;
	uint16 acquisition[128];
	Gradient gradient;
	int8 offset;
}Linescan;

/**
 * DEBUG USING LCD
 * 
 * Data are displayed on the 1st digit (Position of the X)
 * 
 * 		 1st,2nd,3rd digit
 * 		  |   |   |
 * 		  v   v   v
 * 		 ___ ___ ___
 * 		|   |   |   |
 * 		| X | 0 | 0 |
 * 		|___|___|___|
 * 		
 * 	Data can be :
 * 	- 0 : No problem
 * 	
 * 	- 1 : Not enough contrast (max or min is under the value GRADIENT_MINIMUM_VALUE)
 * 	
 * 	- 2 : Data give a line width too small or large to be the reel one (not between 
 * 		MINIMUM_WIDTH and MAXIMUM_WIDTH)
 * 		
 * 	- 3 : Data indicate that the "line" found is too far away for the old one : jump
 * 		(old line position - MAXIMUM_LINE_MOVEMENT_ALLOWED > actual line position)
 * 		
 * 	- 4 : 4 to 6 peak have been detected above the GRADIENT_MINIMUM_VALUE but it's 
 * 		not finish line.
 * 		
 * 	- 5 : v2 only. The line havent reach the offset yet (in order to avoid noise)
 * 		
 */


void gradient_compute(uint16 * acquisition_camera, int32 * gradient);
void gradient_computeLineData(int32 * gradient);
void gradient_moyenneMobile(int32 * signal, int32 * treated_signal);
void signalProcessing(uint16 * acquisition_camera);
uint8 gradient_peakDetection(int32 * signal, Peak * peak, uint8 threshold);
void gradient_moyenneMobile3(int32 * signal, int32 * treated_signal);
void init_line(void);
void init_linescan(uint8 channel , uint16 * line);
void init_sensor(void);
//void gradient_checkIfFinishLine(uint8 numberofPeak, Peak * peak);
//int gradient_checkIfFinishLine_condition(int8 firstPeak, int8 lastPeak, Peak * peak, uint8 version);

#endif /* SIGNALPROCESSING_H_ */
