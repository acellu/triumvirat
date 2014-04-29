#ifndef IRSENSOR_H_
#define IRSENSOR_H_

#include "TFC/TFC.h"
#include "FSL/FSL.h"

/********************************
 * 			IR sensor           *
 ********************************/
// Masque emplacement des bits de controle du mux
#define CTRL_MUX_MASK 0x0C40
// Masque de mise a zero des bits d'adresse
#define IR_SENSOR_ADDR_RESET GPIOC_PDOR &= ~0x0C40
// Adresses des capteurs
#define IR_SENSOR_ADDR_0 GPIOC_PDOR |= 0x0000
#define IR_SENSOR_ADDR_1 GPIOC_PDOR |= 0x0040
#define IR_SENSOR_ADDR_2 GPIOC_PDOR |= 0x0400
#define IR_SENSOR_ADDR_3 GPIOC_PDOR |= 0x0440
#define IR_SENSOR_ADDR_4 GPIOC_PDOR |= 0x0800
#define IR_SENSOR_ADDR_5 GPIOC_PDOR |= 0x0840
#define IR_SENSOR_ADDR_6 GPIOC_PDOR |= 0x0C00
#define IR_SENSOR_ADDR_7 GPIOC_PDOR |= 0x0C40

///Sensor
typedef struct{
	uint16 index[8]; //value between : [0 - 4095]
	uint8 binarizedIndex[8]; //0 for white and 1 for black
	uint16 treshold; //treshold between black and white
	uint8 irSensorReady; //bool true when array's value is ready
	uint8 maxBlackCount; //Maximum black to be found before declaring the line as lost
	uint8 minBlackCount;
	Direction lastDirection; //Last know direction of the line
	uint8 isFound; //1 if the line is found else 0
	int16 error; //Error (between -35 and 35)
}Sensor;

void init_irSensor(void);

void init_sensor(void);

void irSensorProcessing(uint16 * irSensor);


void isSensorLedTest(void);

#endif /* IRSENSOR_H_ */
