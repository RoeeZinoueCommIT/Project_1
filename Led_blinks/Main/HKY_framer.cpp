// 
// 
// 

#include "HKY_framer.h"


const int LDR = A0;
const int BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;


uint16 g_HKY_key_cnt;
bool g_HKY_led_toggle_state;

void p_HKY_framer_init(void)
{
	/************** 
	*  INIT GPIO
	***************/
	pinMode(LDR, INPUT);
	pinMode(BUTTON, INPUT);
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);

	/*******************
	*  INIT Serial COM
	********************/
	Serial.begin(9600);

	/*******************
	*  INIT Module verbs
	********************/
	g_HKY_key_cnt = 0;
	g_HKY_led_toggle_state = false;

	Serial.print("HKY module init OK.\n\r");

}

void p_HKY_show_help_menu(void)
{
	Serial.print("Click:0 \tShow help screen.\n\r");
	Serial.print("Click:1 \tToggle RED led.\n\r");
	Serial.print("Click:1 \tToggle RGB led.\n\r");
}

void p_HKY_rgb_toggle(void)
{
	Serial.print("LDR: ");
	Serial.println(analogRead(LDR));
	Serial.print("BUTTON: ");
	Serial.println(digitalRead(BUTTON));

	analogWrite(RED, random(0, 1023));
	analogWrite(GREEN, random(0, 1023));
	analogWrite(BLUE, random(0, 1023));
}

void p_HKY_led_toggle(LED_NUM xi_led)
{
	analogWrite(xi_led, g_HKY_led_toggle_state);
	delay(500);
	g_HKY_led_toggle_state = !g_HKY_led_toggle_state;
}

bool p_HKY_button_pressed(void)
{
	return digitalRead(BUTTON) == LOW ? true: false;
}

