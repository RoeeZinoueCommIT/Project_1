// HKY_framer.h

#ifndef _HKY_FRAMER_h
#define _HKY_FRAMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	
#include "arduino.h"


typedef enum
{
	RED_LED = 0x2,
	MAX_LED
}LED_NUM;

void p_HKY_framer_init(void);
void p_HKY_show_help_menu(void);
void p_HKY_rgb_toggle(void);
void p_HKY_led_toggle(LED_NUM led);
bool p_HKY_button_pressed(void);

#else
	#include "WProgram.h"
#endif


#endif

