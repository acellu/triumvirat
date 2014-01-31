#ifndef IHM_H_
#define IHM_H_

/*
 * Definition of all the location of the leds
 */
/// Definition of the location of : PTC7  - Orange LED
#define LOC_LED_ORANGE	0x080
/// Definition of the location of : PTC8  - Yellow LED
#define LOC_LED_YELLOW	0x100 
/// Definition of the location of : PTC9  - Green LED
#define LOC_LED_GREEN	0x200 
/// Definition of the location of : PTC11 - Blue LED
#define LOC_LED_BLUE	0x800

/*
 * Definition of all loctaion of the push buttons
 */
/// Definition of the location of the push button : PTC5  - IRQ0
#define LOC_SW3 	0x0020
/// Definition of the location of the push button : PTC13 - IRQ1
#define LOC_SW4 	0x2000 

/**
* Enumeration of the leds for the usage in the other fonction 
*/
typedef enum{
	Orange=0, 	/*!< Orange LED */
	Yellow,		/*!< Yellow LED */
	Green,		/*!< Green LED */
	Blue		/*!< Blue LED */
}LED;

/**
 * Initialise the GPIO for the leds and the push buttons
 */
void init_ihm(void);

/**
 * PortD interruption service
 */
extern void isr_portC(void);

/**
 * Enable the interruption of the push button named SW3 on the board
 */
void ihm_sw3_it_enable(void);

/**
 * Disable the interruption of the push button named SW3 on the board
 */
void ihm_sw3_it_disable(void);

/**
 * Enable the interruption of the push button named SW4 on the board
 */
void ihm_sw4_it_enable(void);

/**
 * Disable the interruption of the push button named SW4 on the board
 */
void ihm_sw4_it_disable(void);

/**
 * Switch a led to an off state.
 * \param led : the led to switch off (structure LED)
 */
void ihm_led_off(LED led);

/**
 * Switch a led to an on state.
 * \param led : the led to switch on (structure LED)
 */
void ihm_led_on(LED led);

#endif /* IHM_H_ */
