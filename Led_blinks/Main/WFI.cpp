
#include "WFI.h"
#include "UTL.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define C_WFI_SSID_MAX_BYTES		(20u)
#define C_WFI_PASS_MAX_BYTES		(20u)
#define C_WFI_CONNECT_CARIEGE_RET	(0x14)

const char* ssid_t = "RzWifi";
const char* pass_t = "30011987";

const char* ssid_ap = "ROEE_ADRUINO";
const char* pass_ap = "1234";

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void p_WFI_init(void)
{
	

	Serial.print("WFI module init OK.\n\r");
}

void p_WFI_print_test(void)
{
	Serial.print("WFI print from module.\n\r");
}

void p_WFI_read_status(void)
{
	Serial.println("Connection:");

	if (WiFi.status() == WL_CONNECTED)
	{
		Serial.println("Connected \n\r");
	}
	else
	{
		Serial.println("Not connected \n\r");
	}

	/* Print local IPv4 address */
	Serial.printf("IP address:\t");
	Serial.println(WiFi.localIP());

	/* Print RSSI */
	Serial.printf("RSSI: %d [dBm]\n\r", WiFi.RSSI());

	/* Print MAC address */
	Serial.printf("MAC:\t");
	Serial.println(WiFi.macAddress());
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

void p_WFI_disconnect(void)
{
	Serial.println("Disconnect from WIFI network ...");
	WiFi.disconnect();
}

void p_WFI_start_ap(void)
{
	bool flag_connect = false;
	flag_connect = WiFi.softAP("AParduino_01", "AP-1-2-3");
	if (true == flag_connect)
	{
		Serial.println("A.P establish O.K, A.P IPV4:");
		Serial.println(WiFi.softAPIP());
	}
	else if (false == flag_connect)
	{
		Serial.println("Fail to establish A.P \n\r ");
	}
	
	/* Begin web server using mDSN - connection*/
	// server.on()

	
}
