
#ifndef _APP_FRAMER_h
#define _APP_FRAMER_h

#if defined(ARDUINO) && ARDUINO >= 100

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "arduino.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ENUMS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

typedef enum
{
	APP_CYCLE_CONTINUE	= 0x0,
	APP_CYCLE_STOP
}APP_CYCLE;

typedef enum
{
	APP_ERR_NO_ERR		= 0x0,
	APP_ERR_EXIT_PROG,
	APP_ERR_REST_PROG,
	APP_ERR_MAX
}APP_ERR;

typedef enum
{
	MAIN_BASE			= 0x0,
	MAIN_HELP,
	MAIN_GPIO_MENU,
	MAIN_WIFI_MENU,
	MAIN_SPIFFS_MENU,
	MAIN_REST_PROG,
	MAIN_EXIT_PROG,
	MAIN_MENU_OPT_MAX
}SERIAL_MAIN_MENU_OPT;

typedef enum
{
	GPIO_BASE			= 0x20,
	GPIO_MENU,
	GPIO_TURN_LED,
	GPIO_RGB_TOGGLE,
	GPIO_TO_MAIN_MENU,
	GPIO_MENU_OPT_MAX
}SERIAL_GPIO_MENU_OPT;

typedef enum
{
	WIFI_BASE			= 0x30,
	WIFI_MENU,
	WIFI_CONNECT,
	WIFI_SHOW_INFO,
	WIFI_DISCONNECT,
	WIFI_START_AP,
	WIFI_LISTEN_HTTP_CLIENTS,
	WIFI_TO_MAIN_MENU,
	WIFI_MENU_OPT_MAX
}SERIAL_WIFI_MENU_OPT;


typedef enum
{
	SPIFFS_BASE			= 0x40,
	SPIFFS_MENU,
	SPIFFS_LIST,
	SPIFFS_FORMAT,
	SPIFFS_TO_MAIN_MENU,
	SPIFFS_MENU_OPT_MAX
}SERIAL_SPIFFS_MENU_OPT;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_APP_framer_init(void);
APP_ERR p_APP_cyclic_main(APP_CYCLE c_cont);

#else
	#include "WProgram.h"
#endif
#endif

