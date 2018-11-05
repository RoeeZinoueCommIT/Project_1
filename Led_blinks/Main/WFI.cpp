
#include "WFI.h"
#include "UTL.h"
#include <ESP8266WiFi.h>

#define C_WFI_SSID_MAX_BYTES		(20u)
#define C_WFI_PASS_MAX_BYTES		(20u)
#define C_WFI_CONNECT_CARIEGE_RET	(0x14)

const char* ssid_t = "M and R";
const char* pass_t = "0542580920";

void p_WFI_init(void)
{
	

	Serial.print("WFI module init OK.\n\r");
}

void p_WFI_print_test(void)
{
	Serial.print("WFI print from module.\n\r");
}


void p_WFI_connect(void)
{
	char ssid[C_WFI_SSID_MAX_BYTES];
	char pass[C_WFI_PASS_MAX_BYTES];

	/* Get SSID from user */
	Serial.println("Please enter SSID:");
	memcpy(ssid, p_UTL_read_str_to_carriage(C_WFI_SSID_MAX_BYTES), C_WFI_SSID_MAX_BYTES);
	
	/* Get Network Pass from user */
	Serial.println("Please enter PASS:");
	memcpy(pass, p_UTL_read_str_to_carriage(C_WFI_PASS_MAX_BYTES), C_WFI_PASS_MAX_BYTES);


	Serial.println("Connecting to SSID: ");
	Serial.println(ssid);
	Serial.println("PASS: ");
	Serial.println(pass);

	WiFi.begin(ssid_t, pass_t);
	// put your setup code here, to run once:
	while (WiFi.status() != WL_CONNECTED) 
	{
		delay(500);
		Serial.print(".");
	}

	Serial.println("Connect to: " + String(ssid_t));
}


void p_WFI_read_status(void)
{
	if (WiFi.status() == WL_CONNECTED)
	Serial.print("IP address:\t");
	Serial.println(WiFi.localIP());
}