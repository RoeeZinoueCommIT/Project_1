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
	Serial.begin(C_HKY_DEBUG_BAUD_RATE);

	/*******************
	*  INIT Module verbs
	********************/
	g_HKY_key_cnt = 0;
	g_HKY_led_toggle_state = false;

	Serial.print("HKY module init OK.\n\r");

}

void p_HKY_show_help_menu(void)
{
	Serial.println(" ********************************* ");
	Serial.println("1 \tShow help screen.\n\r");
	Serial.println("2 \tToggle RED led.\n\r");
	Serial.println("3 \tToggle RGB led.\n\r");
	Serial.println("4 \tTest WIFI.\n\r");
	Serial.println("5 \tTest echo");
	Serial.println(" ********************************* ");
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
	uint16 led_cnt = 0x0;

	Serial.println("Test led: " + String(xi_led) + "begin, Toggle times = " + String(C_HKY_LED_TEST_TOGGLE_TIME));
	for (; led_cnt < C_HKY_LED_TEST_TOGGLE_TIME; led_cnt++)
	{
		analogWrite(xi_led, HIGH);
		delay(500);
		analogWrite(xi_led, LOW);
		delay(500);
		Serial.println(String(led_cnt));
	}
	
}

bool p_HKY_button_pressed(void)
{
	return digitalRead(BUTTON) == LOW ? true: false;
}

