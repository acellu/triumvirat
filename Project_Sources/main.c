#include "common.h"
#include "twrpi_slcd.h"
#include "pwm.h"
#include "camera.h"
#include "ihm.h"
#include "adc16.h"
#include "settings.h"

/*
 * Notes sur l'état du code :
 * 
 * o I du PI desactive
 * o CAMERA_WAITING_TIME modifier
 * o voir influence temps d'attente sur les donnees
 * o traitement du signal !!!
 * o valeur correcteur
 * 
 */


/**
 * Function that initialise clock
 */
void init_clock(void){
	// If the internal load capacitors are being used, they should be selected 
	// before enabling the oscillator. Application specific. 16pF and 8pF selected
	// in this example
	  OSC_CR = OSC_CR_SC16P_MASK | OSC_CR_SC8P_MASK;
	// Enabling the oscillator for 8 MHz crystal
	// RANGE=1, should be set to match the frequency of the crystal being used
	// HGO=1, high gain is selected, provides better noise immunity but does draw
	// higher current
	// EREFS=1, enable the external oscillator
	// LP=0, low power mode not selected (not actually part of osc setup)
	// IRCS=0, slow internal ref clock selected (not actually part of osc setup)
	  MCG_C2 = MCG_C2_RANGE(1) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
	// Select ext oscillator, reference divider and clear IREFS to start ext osc
	// CLKS=2, select the external clock source 
	// FRDIV=3, set the FLL ref divider to keep the ref clock in range 
	//         (even if FLL is not being used) 8 MHz / 256 = 31.25 kHz         
	// IREFS=0, select the external clock 
	// IRCLKEN=0, disable IRCLK (can enable it if desired)
	// IREFSTEN=0, disable IRC in stop mode (can keep it enabled in stop if desired)
	  MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);
	// wait for oscillator to initialize
	  while (!(MCG_S & MCG_S_OSCINIT_MASK)){}  
	// wait for Reference clock to switch to external reference 
	  while (MCG_S & MCG_S_IREFST_MASK){} 
	// Wait for MCGOUT to switch over to the external reference clock 
	  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}
	// Now configure the PLL and move to PBE mode
	// set the PRDIV field to generate a 4MHz reference clock (8MHz /2)
	  MCG_C5 = MCG_C5_PRDIV(1); // PRDIV=1 selects a divide by 2
	// set the VDIV field to 0, which is x24, giving 4 x 24  = 96 MHz
	// the PLLS bit is set to enable the PLL
	// the clock monitor is enabled, CME=1 to cause a reset if crystal fails  
	// LOLIE can be optionally set to enable the loss of lock interrupt
	  
	  MCG_C6 = MCG_C6_CME_MASK | MCG_C6_PLLS_MASK;
	// wait until the source of the PLLS clock has switched to the PLL  
	  while (!(MCG_S & MCG_S_PLLST_MASK)){}
	// wait until the PLL has achieved lock
	  while (!(MCG_S & MCG_S_LOCK_MASK)){}
	// set up the SIM clock dividers BEFORE switching to the PLL to ensure the
	// system clock speeds are in spec.
	// core = PLL (96MHz), bus = PLL/2 (48MHz), flexbus = PLL/2 (48MHz), flash = PLL/4 (24MHz)  
	  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) 
	              | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(3);
	  
	// Transition into PEE by setting CLKS to 0
	// previous MCG_C1 settings remain the same, just need to set CLKS to 0
	  MCG_C1 &= ~MCG_C1_CLKS_MASK;
	// Wait for MCGOUT to switch over to the PLL
	  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){}
	// The USB clock divider in the System Clock Divider Register 2 (SIM_CLKDIV2) 
	// should be configured to generate the 48 MHz USB clock before configuring 
	// the USB module.
	    SIM_CLKDIV2 |= SIM_CLKDIV2_USBDIV(1) | SIM_CLKDIV2_I2SDIV(7); // sets USB divider to /2 assuming reset
											// state of the SIM_CLKDIV2 register

	    SIM_SCGC4|=(SIM_SCGC4_USBOTG_MASK);             // USB Clock Gating
	       
}

/**
 * Function that activate the clock for all GPIO
 */
void init_gpio(void){
	
	//Clock enable on all ports
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
				  | SIM_SCGC5_PORTB_MASK
				  | SIM_SCGC5_PORTC_MASK
				  | SIM_SCGC5_PORTD_MASK
				  | SIM_SCGC5_PORTE_MASK );
}


void main_initialisation(void){
	/* Initialise clock */
	init_clock();
	
	/* Initialise GPIO */
	init_gpio();
	
	/* Initialise the IHM */
	init_ihm();
	
	/* Initialise the timer for the sample time */
	init_sample_time();
	
	/* Initialise TWRPI-SLCD on TWR-K40X256 board*/
	init_twrpi_lcd();
	tlcd_all_off();
  
	//Update SLCD
	tlcd_set_numeric3(0);
	tlcd_set_numeric2(0);
	tlcd_set_numeric1(0);
  
	/* Initialise pwm   */
	init_motor_pwm();
	
	set_left_pwm(0);
	set_right_pwm(0);

	enable_right_motor();
	enable_left_motor();
	
	init_servo_pwm();
	set_servo_angle(0);

	init_camera();

	//Switch on the green led and print 001 on the LCD to confirm that the startup is done
	ihm_led_on(Green);
	tlcd_set_numeric1(0);
	tlcd_set_numeric2(0);
	tlcd_set_numeric3(1);

}

/********************************************************************/
void main (void){
	
	main_initialisation();
	
	while(1)
	{	
		fsm();
	}

}
/********************************************************************/
