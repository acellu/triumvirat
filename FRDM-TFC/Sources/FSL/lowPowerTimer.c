
#include "TFC/TFC.h"
#include "FSL/FSL.h"

void lowPowerTimer_init(void){
	//Enable cloking
	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
	
	/* Reset LPTMR settings */
	LPTMR0_CSR=0;
	
	//Prescaler
	LPTMR0_PSR = LPTMR_PSR_PCS(1)|LPTMR_PSR_PBYP_MASK;

}

void lowPowerTimer_setTime(uint16 temps){
	//Setting the value
	LPTMR0_CMR = LPTMR_CMR_COMPARE(temps);
	
	//Start the timer
	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
}
