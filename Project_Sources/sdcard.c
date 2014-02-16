#include "common.h"

void sdcard_init(void){
	//GPIO Initialisation
	PORTE_PCR0  |= PORT_PCR_MUX(4);	//SD Data 1
	PORTE_PCR1  |= PORT_PCR_MUX(4);	//SD Data 0
	PORTE_PCR2  |= PORT_PCR_MUX(4);	//SD Clock
	PORTE_PCR3  |= PORT_PCR_MUX(4);	//SD Command
	PORTE_PCR4  |= PORT_PCR_MUX(4);	//SD Data 3
	PORTE_PCR5  |= PORT_PCR_MUX(4);	//SD Data 2
	PORTE_PCR27 |= PORT_PCR_MUX(1);	//SD Write Protect
	PORTA_PCR16 |= PORT_PCR_MUX(1);	//SD Card Detect

	//Clocking enable (system clock)
	SIM_SOPT2 |= SIM_SOPT2_SDHCSRC(0);

}
