/****************************************************************
* Desctiption:  
* Module name: WFI
* Version: 1_001
* Date: 20-11-2018
****************************************************************/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>


#include "WFI_framer.h"
#include "HKY_framer.h"
#include "SFS_framer.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#define C_WFI_SSID_MAX_BYTES		(20u)
#define C_WFI_PASS_MAX_BYTES		(20u)
#define C_WFI_CONNECT_CARIEGE_RET	(0x14)

#define C_WFI_MDNS_NAME				("WFI")

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

const char* ssid_ap = "ROEE_ADRUINO";
const char* pass_ap = "1234";

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
WebSocketsServer webSocket(81);    // create a websocket server on port 81
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'


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
bool p_WFI_send_page_from_fs(String xi_file_name);

/* Web-socket events */

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght);

const int LEDPIN = 0;
// Current LED status
bool LEDStatus;

// Commands sent through Web Socket
const char LEDON[] = "ledon";
const char LEDOFF[] = "ledoff";

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_WFI_framer_init(void)
{
	WiFi.mode(WIFI_AP_STA);

	p_WFI_start_wifi_multi();
	p_WFI_start_mdns();
	p_WFI_start_server();
	p_WFI_print_connect_info();

	/* Web socket init */
	webSocket.begin();                          // start the websocket server
	webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
	Serial.println("WebSocket server started.");

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
	wifiMulti.addAP("M and R", "0542580920");

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


	//Serial.println("Connecting to SSID: ");
	//Serial.println(ssid);
	//Serial.println("PASS: ");
	//Serial.println(pass);

	//WiFi.begin(ssid_t, pass_t);
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
	p_WFI_send_page_from_fs("GPIO_web.html");
	
}



void p_WFI_server_handle_first_page()
{
	p_WFI_send_page_from_fs("GPIO_web.html");
}

void handleNotFound() 
{
	server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void p_WFI_listen_http_client(void)
{
	webSocket.loop();
	server.handleClient();                    // Listen for HTTP requests from clients
}

bool p_WFI_send_page_from_fs(String xi_file_name)
{
	File file;
	String file_suffix;
	bool ret = false;

	ret = p_SFS_get_file(xi_file_name, &file, &file_suffix);
	
	if (true == ret)
	{
		server.streamFile(file, file_suffix);
		file.close();

		Serial.println("File: " + xi_file_name + " found and load.\n\r");
	}
	else
	{
		Serial.println("File: " + xi_file_name + " Not found in SPIFFS storage");
		Serial.println("File: " + xi_file_name + " not found.\n\r");
		server.send(404, "text/plain", "404: Not found"); 
	}

	return (ret);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
	Serial.printf("webSocketEvent(%d, %d, ...)\r\n", num, type);
	switch (type) 
	{
		case WStype_DISCONNECTED:
			Serial.printf("[%u] Disconnected!\r\n", num);
			break;
	
		case WStype_CONNECTED:
		{
			IPAddress ip = webSocket.remoteIP(num);
			Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\r\n", num, ip[0], ip[1], ip[2], ip[3], payload);
			// Send the current LED status
			if (LEDStatus) 
			{
				webSocket.sendTXT(num, LEDON, strlen(LEDON));
			}
			else 
			{
				webSocket.sendTXT(num, LEDOFF, strlen(LEDOFF));
			}
		}
		break;
	
		case WStype_TEXT:
			Serial.printf("[%u] get Text: %s\r\n", num, payload);

			if (strcmp(LEDON, (const char *)payload) == 0) 
			{
				p_HKY_led_toggle(C_HKY_GPIO_LED_BLUE, 1);
				webSocket.sendTXT(num, LEDON, strlen(LEDON));
			}
			else if (strcmp(LEDOFF, (const char *)payload) == 0) 
			{
				p_HKY_led_toggle(C_HKY_GPIO_LED_BLUE, 4);
				webSocket.sendTXT(num, LEDOFF, strlen(LEDOFF));
			}
			else 
			{
				Serial.println("Unknown command");
			}
			// send data to all connected clients
			webSocket.broadcastTXT(payload, length);
			break;
	
		case WStype_BIN:
			Serial.printf("[%u] get binary length: %u\r\n", num, length);
			hexdump(payload, length);

			// echo data back to browser
			webSocket.sendBIN(num, payload, length);
			break;
	
		default:
			Serial.printf("Invalid WStype [%d]\r\n", type);
			break;
	}
}
