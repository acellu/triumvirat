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

void init_irSensor(void);

void init_sensor(void);

#endif /* IRSENSOR_H_ */
