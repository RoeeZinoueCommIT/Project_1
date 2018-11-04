// 
// 
// 

#include "WFI.h"

const char* ssid = "VaasaHacklab";
const char* pass = "wifikey";

void p_WFI_init(void)
{
	//WiFi.begin(ssid, pass);

	Serial.print("WFI module init OK.\n\r");
}

void p_WFI_print_test(void)
{
	Serial.print("WFI print from module.\n\r");
}