/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>

#include "WFI_framer.h"
#include "HKY_framer.h"


/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#define C_WFI_SSID_MAX_BYTES		(20u)
#define C_WFI_PASS_MAX_BYTES		(20u)
#define C_WFI_CONNECT_CARIEGE_RET	(0x14)

#define C_WFI_MDNS_NAME				("WFI")
/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

const char* ssid_t = "Comm-it Embedded";
const char* pass_t = "klop2233";

const char* ssid_ap = "ROEE_ADRUINO";
const char* pass_ap = "1234";

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

const char INDEX_HTML[] =
"<!DOCTYPE HTML>"
"<html>"
"<head>"
"<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0, maximum-scale = 1.0, user-scalable=0\">"
"<title>ESP8266 Web Form Demo</title>"
"<style>"
"\"body { background-color: #808080; font-family: Arial, Helvetica, Sans-Serif; Color: #000000; }\""
"</style>"
"</head>"
"<body>"
"<h1>ESP8266 Relay Test Web Form</h1>"
"<FORM action=\"/\" method=\"post\">"
"<P>"
"Relay<br>"
"<INPUT type=\"text\" name=\"delay\"<BR>"
"<INPUT type=\"submit\" value=\"Send\"> <INPUT type=\"reset\">"
"</P>"
"</FORM>"
"</body>"
"</html>";


const char C_WPI_FIRST_PAGE[] = 
"<!DOCTYPE html>"
"<html>"
"<body>"
"<h2>Main page</h2>"
"<FORM action=\"/\" method=\"post\">"
"<input type='submit' name='Toggle GPIO' value='Toggle Blue led' formAction=\"/LED\"/>"
"<BR>"
"<input type='submit' name='update' value='View system statistics' formAction=\"/STAT\"/>"
"<BR>"
"</FORM>"
"<br><br><br><br> <footer><p>Developed by: Roee Z</p><p><a href=\"mailto:someone@example.com\">znroee@gmail.com</a>.</p></footer>"
"</body>"
"</html>";

const char C_WPI_STAT_PAGE[] =
"<!DOCTYPE html>"
"<html>"
"<body>"
"<h2>System statistic page</h2>"
"<FORM action=\"/\" method=\"post\">"
"<p align ='left' style='font-size:160%'><b> System run time:<input type='text' name='USERNAME' placeholder='user name' required></b></p><br>"
"<p align ='left' style='font-size:160%'><b> GPIOs state:<input type='text' name='USERNAME' placeholder='user name' required></b></p><br>"
"<input type='submit' name='update' value='Back to main page' formAction=\"/MANU\"/>"
"<BR>"
"</FORM>"
"</body>"
"</html>";
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% LOCAL DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/* Init functions */
bool p_WFI_start_mdns(void);
bool p_WFI_start_wifi_multi(void);
bool p_WFI_start_server(void);

/* Server handler */
void handleRoot(void);
void p_WFI_server_handle_led(void);
void p_WFI_server_handle_statistic(void);
void handleNotFound(void);
void handleRoot(void);
void handleNotFound(void);

/* General methods */
void p_WFI_server_handle_first_page(void);
void p_WFI_print_connect_info(void);


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_WFI_init(void)
{
	WiFi.mode(WIFI_AP_STA);

	p_WFI_start_wifi_multi();
	p_WFI_start_mdns();
	p_WFI_start_server();
	p_WFI_print_connect_info();

	Serial.println("WFI module init O.K \n\r");
}

bool p_WFI_start_mdns()
// Start the mDNS responder
{ 
	bool res = false;
	res = MDNS.begin(C_WFI_MDNS_NAME);
	if (true == res)
	{
		Serial.print("mDNS responder started: http://");
		Serial.print(C_WFI_MDNS_NAME);
		Serial.println(".local");
	}
	else
	{
		Serial.println("Error setting up MDNS responder!");
	}

	return (res);
}

bool p_WFI_start_wifi_multi()
{
	bool res = false;

	wifiMulti.addAP("Comm-it Embedded", "klop2233");

	while (wifiMulti.run() != WL_CONNECTED)
	{
		
		delay(1000);
		Serial.print('.');
		res = true;
	}

	return (res);
}

bool p_WFI_start_server()
{
	// Start TCP (HTTP) server
	server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
	server.on("/LED", HTTP_POST, p_WFI_server_handle_led);
	server.on("/STAT", HTTP_POST, p_WFI_server_handle_statistic);
	server.on("/MANU", HTTP_POST, p_WFI_server_handle_first_page);
	
	server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

	server.begin();                           // Actually start the server
	Serial.println("HTTP server started");
}

void p_WFI_print_connect_info(void)
{
	Serial.println("Connected to SSID: \n\r");
	Serial.println(WiFi.SSID());
	Serial.print("Target IP_V4 address:\t");
	Serial.println(WiFi.localIP());
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
	//memcpy(ssid, p_UTL_read_str_to_carriage(C_WFI_SSID_MAX_BYTES), C_WFI_SSID_MAX_BYTES);
	
	/* Get Network Pass from user */
	Serial.println("Please enter PASS:");
	//memcpy(pass, p_UTL_read_str_to_carriage(C_WFI_PASS_MAX_BYTES), C_WFI_PASS_MAX_BYTES);


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
}



void handleRoot() 
// When URI / is requested, send a web page with a button to toggle the LED
{
	p_WFI_server_handle_first_page();
}

void p_WFI_server_handle_led()
{                          
	Serial.println("Hello");
	p_HKY_led_toggle(C_HKY_GPIO_LED_BLUE, 2);
	server.sendHeader("Location", "/");        // Add a header to respond with a new location for the browser to go to the home page again
	server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void p_WFI_server_handle_statistic()
{           
	server.send(200, "text/html", C_WPI_STAT_PAGE);
}

void p_WFI_server_handle_first_page()
{
	server.send(200, "text/html", C_WPI_FIRST_PAGE);
}

void handleNotFound() 
{
	server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void p_WFI_listen_http_client(void)
{
	server.handleClient();                    // Listen for HTTP requests from clients
}
