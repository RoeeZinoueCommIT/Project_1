/****************************************************************
* Description: Application top level handler module.
* Module name: APP.
* Version: 1_001
* Date: 29-10-2018
****************************************************************/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "APP_framer.h"
#include "WFI_framer.h"
#include "HKY_framer.h"
#include "SFS_framer.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

uint8_t g_APP_disp_opt;
uint8_t g_APP_opt_offset;
uint8_t g_APP_prev_key;
APP_ERR g_APP_err;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% LOCAL DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

static void p_APP_main_fsm(void);
static void p_APP_wifi_fsm(void);
static void p_APP_gpio_fsm(void);
static void p_APP_spiffs_fsm(void);

static void p_APP_show_help_menu(void);
static void p_APP_show_gpio_menu(void);
static void p_APP_show_wifi_menu(void);
static void p_APP_show_spiffs_menu(void);

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup APP
\brief Interface function:
The function init application module.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void p_APP_framer_init(void)
{
	Serial.println("System boot ok");
	Serial.println("Compile time: " + String(__TIME__) + " Date: " + String(__DATE__));
	delay(500);

	g_APP_opt_offset = WIFI_BASE;
	g_APP_disp_opt = WIFI_LISTEN_HTTP_CLIENTS;
	g_APP_err = APP_ERR_NO_ERR;
	g_APP_prev_key = 0x0;
	
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup APP
\brief Interface function:
The function shall manage application cyclic functions.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
APP_ERR p_APP_cyclic_main(APP_CYCLE c_cont)
{
	if (c_cont == APP_CYCLE_CONTINUE)
	{
		if (Serial.available())
		{
			g_APP_disp_opt = Serial.read() - '0' + g_APP_opt_offset;
		}

		p_APP_gpio_fsm();
		p_APP_main_fsm();
		p_APP_wifi_fsm();
		p_APP_spiffs_fsm();
	}
	return (g_APP_err);
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup APP
\brief Local function:
The function shall manage main FSM.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void p_APP_main_fsm(void)
{
	switch (g_APP_disp_opt)
	{
		case MAIN_BASE:
			break;

		case MAIN_HELP:
			p_APP_show_help_menu();
			g_APP_disp_opt = MAIN_BASE;
			break;

		case MAIN_GPIO_MENU:
			g_APP_opt_offset = GPIO_BASE;
			g_APP_disp_opt = GPIO_MENU;
			break;

		case MAIN_WIFI_MENU:
			g_APP_opt_offset = WIFI_BASE;
			g_APP_disp_opt = WIFI_MENU;
			break;

		case MAIN_SPIFFS_MENU:
			g_APP_opt_offset = SPIFFS_BASE;
			g_APP_disp_opt = SPIFFS_MENU;
			break;

		case MAIN_REST_PROG:
			Serial.println("Reset program");
			delay(500);
			g_APP_err = APP_ERR_REST_PROG;
			break;

		case MAIN_EXIT_PROG:
			Serial.println("Terminate program");
			delay(500);
			g_APP_err = APP_ERR_EXIT_PROG;
			break;
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup APP
\brief Local function:
The function shall manage GPIO FSM.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void p_APP_gpio_fsm(void)
{
	switch (g_APP_disp_opt)
	{
		case GPIO_BASE:
			break;

		case GPIO_MENU:
			p_APP_show_gpio_menu();
			g_APP_disp_opt = GPIO_BASE;
			break;

		case GPIO_TURN_LED:
			p_HKY_led_toggle(C_HKY_GPIO_LED_BLUE, 5);
			g_APP_disp_opt = GPIO_MENU;
			break;

		case GPIO_RGB_TOGGLE:
			p_HKY_rgb_toggle();
			g_APP_disp_opt = GPIO_MENU;
			break;

		case GPIO_TO_MAIN_MENU:
			g_APP_opt_offset = MAIN_BASE;
			g_APP_disp_opt = MAIN_HELP;
			break;
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup APP
\brief Local function:
The function shall manage WIFI FSM.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void p_APP_wifi_fsm(void)
{
	switch (g_APP_disp_opt)
	{
		case WIFI_BASE:
			break;

		case WIFI_MENU:
			p_APP_show_wifi_menu();
			g_APP_disp_opt = WIFI_BASE;
			break;

		case WIFI_CONNECT:
			p_WFI_connect();
			g_APP_disp_opt = WIFI_MENU;
			break;

		case WIFI_SHOW_INFO:
			p_WFI_read_status();
			g_APP_disp_opt = WIFI_MENU;
			break;

		case WIFI_DISCONNECT:
			p_WFI_disconnect();
			break;

		case WIFI_START_AP:
			p_WFI_start_ap();
			g_APP_disp_opt = WIFI_LISTEN_HTTP_CLIENTS;
			break;

		case WIFI_LISTEN_HTTP_CLIENTS:
			p_WFI_listen_http_client();
			break;

		case WIFI_TO_MAIN_MENU:
			g_APP_opt_offset = MAIN_BASE;
			g_APP_disp_opt = MAIN_HELP;
			break;
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup APP
\brief Local function:
The function manage Flash file system FSM.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void p_APP_spiffs_fsm(void)
{
	switch (g_APP_disp_opt)
	{
		case SPIFFS_BASE:
			break;
			
		case SPIFFS_MENU:
			p_APP_show_spiffs_menu();
			g_APP_disp_opt = SPIFFS_BASE;
			break;

		case SPIFFS_LIST:
			p_SFS_list_all_files();
			g_APP_disp_opt = SPIFFS_BASE;
			break;

		case SPIFFS_FORMAT:
			p_SFS_fomrat();
			break;

		case SPIFFS_TO_MAIN_MENU:
			g_APP_opt_offset = MAIN_BASE;
			g_APP_disp_opt = MAIN_HELP;
			break;
	}
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup APP
\brief Local function:
The function shall print to console main menu.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void p_APP_show_help_menu(void)
{
	Serial.println("*********** *********** ***********");
	Serial.printf("%d \tShow application help.\n\r", MAIN_HELP);
	Serial.printf("%d \tShow GPIO menu.\n\r", MAIN_GPIO_MENU);
	Serial.printf("%d \tShow WIFI menu.\n\r", MAIN_WIFI_MENU);
	Serial.printf("%d \tShow SPIFFS menu.\n\r", MAIN_SPIFFS_MENU);
	Serial.printf("%d \tSW Reset program.\n\r", MAIN_REST_PROG);
	Serial.printf("%d \tExit from program.\n\r", MAIN_EXIT_PROG);
	Serial.println("*********** *********** *********** \n\r");
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup APP
\brief Local function:
The function shall print to console GPIO menu.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void p_APP_show_gpio_menu(void)
{
	Serial.println("*********** *********** ***********");
	Serial.printf("%d \tShow GPIO help screen.\n\r", GPIO_MENU - GPIO_BASE);
	Serial.printf("%d \tSet GPIO.\n\r", GPIO_TURN_LED - GPIO_BASE);
	Serial.printf("%d \tToggle RGB led.\n\r", GPIO_RGB_TOGGLE - GPIO_BASE);
	Serial.printf("%d \tBack to main menu.\n\r", GPIO_TO_MAIN_MENU - GPIO_BASE);
	Serial.println("*********** *********** *********** \n\r");
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup APP
\brief Local function:
The function shall print to console WIFI menu.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void p_APP_show_wifi_menu(void)
{
	Serial.println("*********** *********** ***********");
	Serial.printf("%d \tShow WIFI help screen.\n\r", WIFI_MENU - WIFI_BASE);
	Serial.printf("%d \tConnect to A.P.\n\r", WIFI_CONNECT - WIFI_BASE);
	Serial.printf("%d \tShow WIFI connection info.\n\r", WIFI_SHOW_INFO - WIFI_BASE);
	Serial.printf("%d \tDisconnect from A.P..\n\r", WIFI_DISCONNECT - WIFI_BASE);
	Serial.printf("%d \tStart new A.P..\n\r", WIFI_START_AP - WIFI_BASE);
	Serial.printf("%d \tListen client requests.\n\r", WIFI_LISTEN_HTTP_CLIENTS - WIFI_BASE);
	Serial.printf("%d \tBack to main menu.\n\r", WIFI_TO_MAIN_MENU - WIFI_BASE);
	Serial.println("*********** *********** *********** \n\r");
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup APP
\brief Local function:
The function shall print to console SPIFFS menu.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void p_APP_show_spiffs_menu(void)
{
	Serial.println("*********** *********** ***********");
	Serial.printf("%d \tShow SPIFFS help screen.\n\r", SPIFFS_MENU -SPIFFS_BASE);
	Serial.printf("%d \tList SPIFFS files.\n\r", SPIFFS_LIST - SPIFFS_BASE);
	Serial.printf("%d \tFormat SPIFFS storage.\n\r", SPIFFS_FORMAT - SPIFFS_BASE);
	Serial.printf("%d \tBack to main menu.\n\r", SPIFFS_TO_MAIN_MENU - SPIFFS_BASE);
	Serial.println("*********** *********** *********** \n\r");
}



