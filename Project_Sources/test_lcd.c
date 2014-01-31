#include "test_lcd.h"
//#include "twrpi_slcd.h"
#include "common.h"


void test_lcd(void){
	init_twrpi_lcd();
	tlcd_all_on();
	test_delay();
	tlcd_degrees_off();
	test_delay();
	tlcd_percent_off();
	test_delay();
	tlcd_am_off();
	test_delay();
	tlcd_pm_off();
	test_delay();
	tlcd_1_off();
	test_delay();
	tlcd_fsl_off();
	test_delay();
	tlcd_colon_off();
	test_delay();
	tlcd_set_numeric1(1);
	test_delay();
	tlcd_set_numeric2(1);
	test_delay();
	tlcd_set_numeric3(1);
}

/*
 * delay the code
 * Only for test uses, this function lock the program
 */
void test_delay(void){
	uint32 k;
	for(k=0; k<=30000000; k++){
	}
}
