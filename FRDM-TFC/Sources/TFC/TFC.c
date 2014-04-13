#include "TFC\TFC.h"


void TFC_Init()
{
	TFC_InitClock(); //Ok
	TFC_InitSysTick(); //Ok
	TFC_InitGPIO(); //Ok
	TFC_InitServos(); //Ok
	TFC_InitMotorPWM(); //Ok
    TFC_InitADCs(); //Ok
    TFC_InitLineScanCamera(); //Ok
    TFC_InitTerminal(); //*
	TFC_InitUARTs(); //*
	
	TFC_HBRIDGE_DISABLE;
	TFC_SetMotorPWM(0,0);
	
}

void TFC_Task()
{
	#if defined(TERMINAL_USE_SDA_SERIAL)
		TFC_UART_Process();
	#endif
	 
    TFC_ProcessTerminal();
}
