/****************************************************************
* Description: Wifi controler interface level module.
* Module name: WFI
* Version: 1_001
* Date: 29-10-2018
****************************************************************/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

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

HTTPClient http;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% LOCAL DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/* Init functions */
bool p_WFI_start_mdns(void);
bool p_WFI_start_wifi_multi(void);
bool p_WFI_manage_client_pages(void);

/* Server handler */
void handleRoot(void);
void p_WFI_server_handle_led(void);
void p_WFI_server_handle_statistic(void);
void handleNotFound(void);
void handleRoot(void);
void handleNotFound(void);

/* General methods */

/* GPIO */
static void Page_html(void);
static void p_WFI_web_send(void);


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
	p_WFI_print_connect_info();
	p_WFI_manage_client_pages();

	/* Start the server */
	server.begin();

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

void p_WFI_print_connect_info(void)
{
	Serial.println("Connected to SSID: \n\r");
	Serial.println(WiFi.SSID());
	Serial.print("Target IP_V4 address:\t");
	Serial.println(WiFi.localIP());
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
		server.send(404, "text/plain", "404: Not found"); 
	}

	return (ret);
}

bool p_WFI_manage_client_pages()
{
	/* Main page */
	server.on("/", HTTP_GET, p_WFI_web_send);
	server.on("/Main.js", HTTP_GET, p_WFI_web_send);
	server.on("/Main.css", HTTP_GET, p_WFI_web_send);

	/* Server not found page */
	server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
}

void handleNotFound()
{
	server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void p_WFI_web_send()
{
	String page = server.uri();
	page.remove(0, 1);

	if (page == "") 
	{
		/* True = Root page */
		p_WFI_send_page_from_fs("Main.html");
	}
	else
	{
		p_WFI_send_page_from_fs(page);
	}
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght)
{
	Serial.println("Hello \n\r");
	switch (type)
	{
	case WStype_DISCONNECTED:             // if the websocket is disconnected
		Serial.printf("[%u] Disconnected!\n", num);
		break;
	case WStype_CONNECTED: {              // if a new websocket connection is established
							   IPAddress ip = webSocket.remoteIP(num);
							   Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
							   //						   rainbow = false;                  // Turn rainbow off when a new connection is established
	}
		break;
	case WStype_TEXT:                     // if new text data is received
		Serial.printf("[%u] get Text: %s\n", num, payload);
		if (payload[0] == '#') {            // we get RGB data
			uint32_t rgb = (uint32_t)strtol((const char *)&payload[1], NULL, 16);   // decode rgb data
			int r = ((rgb >> 20) & 0x3FF);                     // 10 bits per color, so R: bits 20-29
			int g = ((rgb >> 10) & 0x3FF);                     // G: bits 10-19
			int b = rgb & 0x3FF;                      // B: bits  0-9

			//	analogWrite(LED_RED, r);                         // write it to the LED output pins
			// analogWrite(LED_GREEN, g);
			// analogWrite(LED_BLUE, b);
		}
		else if (payload[0] == 'R')
		{                      // the browser sends an R when the rainbow effect is enabled
			// rainbow = true;
		}
		else if (payload[0] == 'N')
		{                      // the browser sends an N when the rainbow effect is disabled
			//rainbow = false;
		}
		break;
	}
}

void p_WFI_file_upload()
{
		//HTTPUpload& upload = server.upload();
		//if (upload.status == UPLOAD_FILE_START) 
		//{
		//	String filename = upload.filename;
		//	if (!filename.startsWith("/")) filename = "/" + filename;
		//	Serial.print("handleFileUpload Name: "); 
		//	Serial.println(filename);
		//	fsUploadFile = SPIFFS.open(filename, "w");            // Open the file for writing in SPIFFS (create if it doesn't exist)
		//	filename = String();
		//}
		//else if (upload.status == UPLOAD_FILE_WRITE) 
		//{
		//	if (fsUploadFile)
		//		fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
		//}
		//else if (upload.status == UPLOAD_FILE_END) 
		//{
		//	if (fsUploadFile) 
		//	{                                    // If the file was successfully created
		//		fsUploadFile.close();                               // Close the file again
		//		Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
		//		server.sendHeader("Location", "/success.html");      // Redirect the client to the success page
		//		server.send(303);
		//	}
		//	else 
		//	{
		//		server.send(500, "text/plain", "500: couldn't create file");
		//	}
}