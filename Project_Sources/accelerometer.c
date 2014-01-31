/*
 * accelerometer.c
 *
 *  Created on: Jan 23, 2014
 *      Author: Axel
 */

#include "accelerometer.h"

// Accelerometer variable
CoordAccelerometer coordAccelero;


/*
 * Initialise I2C, sensor and global structure
 */
void init_acelerometer(void){
  //Initialize I2C
  init_I2C();

  //Configure MMA7660 sensor
  MMA7660WriteRegister(0x09,0xE0); //Disable tap detection
  MMA7660WriteRegister(0x07,0x19); //Enable auto-sleep, auto-wake, and put in active mode
  
  //Initialise structure
  coordAccelero.accelero_x = 0;
  coordAccelero.accelero_y = 0;
  coordAccelero.accelero_z = 0;
}

/*
 * Read and stock axis register into global structure
 */
void accelerometer_update(void)
{ 
	//Read x-axis register
	coordAccelero.accelero_x = convert(u8MMA7660ReadRegister(0x00));
	
	//Read y-axis register
	coordAccelero.accelero_y = convert(u8MMA7660ReadRegister(0x01));
	
	//Read z-axis register
	coordAccelero.accelero_z = convert(u8MMA7660ReadRegister(0x02));
}


/*
 * Convert 6-bit accelerometer result into an 8-bit signed char
 */
signed char convert(signed char input)
{
    input &=~0x40; //Turn off alert bit
    if(input & 0x20)    // update 6-bit signed format to 8-bit signed format
    	input |= 0xE0;
    return (signed char) input;
}

/*
 * Delay function using the LPTMR module
 */
void time_delay_ms(unsigned int count_val)
{
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK; //Turn on clock to LPTMR module
  LPTMR0_CMR = count_val; //Set compare value
  LPTMR0_PSR = LPTMR_PSR_PCS(1)|LPTMR_PSR_PBYP_MASK; //Use 1Khz LPO clock and bypass prescaler
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK; //Start counting

  while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)) {} //Wait for counter to reach compare value

  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //Clear Timer Compare Flag
  return;
}
