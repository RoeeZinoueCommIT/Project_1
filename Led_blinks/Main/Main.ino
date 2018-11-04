#include "HKY_framer.h"
#include "WFI.h"
#include "APP_framer.h"
#include "WIFI.h"



int key = 0x0;   // for incoming serial data
int key_cnt = 0x0;

uint16 g_APP_opt_cnt = 0x0;

typedef enum
{
	DISP_HELP	= 0x0,
	TOGGLE_RED_LED,
	TOGGLE_RGB_LED,
	TEST_WIFI_MODULE,
	MENU_OPT_MAX
}SERIAL_MENU_OPT;

void setup()
{
	p_HKY_framer_init();
	p_WFI_init();
}


void red_toggle(void)
{
	Serial.print("RED LED toggle.\n\r");
}

void loop()
{
	
	if (p_HKY_button_pressed() == true)
	{
		g_APP_opt_cnt++;
		g_APP_opt_cnt %= MENU_OPT_MAX;
	}

	switch (g_APP_opt_cnt)
	{
		case DISP_HELP:
			
			p_HKY_show_help_menu();
			break;
		
		case TOGGLE_RED_LED:
			
			p_HKY_led_toggle(RED_LED);
			break;
		
		case TOGGLE_RGB_LED:
			
			p_HKY_rgb_toggle();
			break;

		case TEST_WIFI_MODULE:
			p_WFI_print_test();
			break;

		default:
			Serial.print("Option default \n\r");
			break;
	}

	delay(2000);
}