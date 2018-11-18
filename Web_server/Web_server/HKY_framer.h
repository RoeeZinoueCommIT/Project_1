
#ifndef _HKY_FRAMER_h
#define _HKY_FRAMER_h

#if defined(ARDUINO) && ARDUINO >= 100

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "arduino.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#define C_HKY_DEBUG_BAUD_RATE		(115200)
#define C_HKY_LED_TOGGLE_TIME_MSEC	(250)

#define C_HKY_GPIO_LED_BLUE (2u)
#define C_HKY_RGB_LED_RED	(15u)
#define C_HKY_RGB_LED_BLUE	(13u)
#define C_HKY_RGB_LED_GREEN (12u)
#define C_HKY_GPIO_BUTTON	(4u)
#define C_HKY_LDR			(A0)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_HKY_framer_init(void);
void p_HKY_rgb_toggle(void);
void p_HKY_led_toggle(uint16 xi_led, uint16 xi_times);
bool p_HKY_button_pressed(void);

#else
	#include "WProgram.h"
#endif


#endif

