/****************************************************************
* Brief
* Module name:
* Version: 1_001
* Date: 29-10-2018
****************************************************************/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "HKY_framer.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

uint16 g_HKY_key_cnt;
bool g_HKY_led_toggle_state;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_HKY_framer_init(void)
{
	/************** 
	*  INIT GPIO
	***************/
	/* LDR LED configuration */
	pinMode(C_HKY_LDR, INPUT);
	pinMode(C_HKY_GPIO_BUTTON, INPUT);
	pinMode(C_HKY_RGB_LED_RED, OUTPUT);
	pinMode(C_HKY_RGB_LED_GREEN, OUTPUT);
	pinMode(C_HKY_RGB_LED_BLUE, OUTPUT);

	/* Blue led configuration */
	pinMode(C_HKY_GPIO_LED_BLUE, OUTPUT);

	/*******************
	*  INIT Serial COM
	********************/
	Serial.begin(C_HKY_DEBUG_BAUD_RATE);

	/*******************
	*  INIT Module verbs
	********************/
	g_HKY_key_cnt = 0;
	g_HKY_led_toggle_state = false;

	Serial.print("HKY module init OK.\n\r");
}

void p_HKY_rgb_toggle(void)
{
	Serial.print("LDR: ");
	Serial.println(analogRead(C_HKY_LDR));
	Serial.print("BUTTON: ");
	Serial.println(digitalRead(C_HKY_GPIO_BUTTON));

	analogWrite(C_HKY_RGB_LED_RED, random(0, 1023));
	analogWrite(C_HKY_RGB_LED_GREEN, random(0, 1023));
	analogWrite(C_HKY_RGB_LED_BLUE, random(0, 1023));
}

void p_HKY_led_toggle(uint16 xi_led, uint16 xi_times)
{
	uint16 led_cnt = 0x0;

	Serial.println("Test led: " + String(xi_led) + " Toggle times = " + String(xi_times) + " begin... ");
	for (; led_cnt < xi_times * 2; led_cnt++)
	{
		digitalWrite(xi_led, !digitalRead(xi_led));
		delay(C_HKY_LED_TOGGLE_TIME_MSEC);
	}
}

bool p_HKY_button_pressed(void)
{
	return digitalRead(C_HKY_GPIO_BUTTON) == LOW ? true: false;
}

