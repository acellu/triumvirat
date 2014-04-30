#ifndef LOWPOWERTIMER_H_
#define LOWPOWERTIMER_H_


#include "TFC/TFC.h"
#include "FSL/FSL.h"

//Variable != de 0 si fini de compter, sinon = 0
#define TIMER_FINISH (LPTMR0_CSR & LPTMR_CSR_TCF_MASK)
#define TIMER_CLEAR_FLAG (LPTMR0_CSR &= ~LPTMR_CSR_TCF_MASK)

void lowPowerTimer_init(void);

//Le temps est en milliseconde, pas d'IT, il faut scruter la valeur
void lowPowerTimer_setTime(uint16 temps);

#endif /* LOWPOWERTIMER_H_ */
