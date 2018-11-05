

#include "UTL.h"
#include "HKY_framer.h"
#include "WFI.h"
#include "APP_framer.h"



typedef enum
{
	DISP_HELP			= 0x1,
	TOGGLE_RED_LED,
	TOGGLE_RGB_LED,
	TEST_WIFI_MODULE,
	TEST_ECHO,
	MENU_OPT_MAX
}SERIAL_MENU_OPT;

void setup()
{
	p_HKY_framer_init();
	p_WFI_init();

	

	Serial.println("System boot ok");
	Serial.println("Compile time: " + String(__TIME__) + " Date: " + String(__DATE__));
}

void loop()
{
	/* Get char from serial */

	if (Serial.available())
	{
		uint8_t ch = Serial.read() - '0';

		switch (ch)
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

				p_WFI_connect();
				break;

			default:
				Serial.print("Option: " + String(ch - '0') + " Not sepported \n\r");
				break;
		}
	}
		
	
	
	
}