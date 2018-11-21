// OFP_framer.h

#ifndef _OFP_FRAMER_h
#define _OFP_FRAMER_h

#if defined(ARDUINO) && ARDUINO >= 100

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "arduino.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% DEFINES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/* WIFI */
#define C_OFP_WIFI_MAX_AP_NET_OBJ       (10u)
#define C_OFP_WIFI_MAX_AP_NET_CHARS     (10u)

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% ENUMS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

typedef enum
{
	E_OFP_CLR_ALL	= 0,
	E_OFP_CLR_WIFI,
	E_OFP_CLR_GPIO
}E_OFP_CLR;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% STRUCTURES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/* WIFI parameters */

typedef struct
{
	char ssid[C_OFP_WIFI_MAX_AP_NET_CHARS];
    char pass[C_OFP_WIFI_MAX_AP_NET_CHARS];
}DT_OFP_wifi_ap_net_info;

typedef struct
{
	DT_OFP_wifi_ap_net_info net_info [C_OFP_WIFI_MAX_AP_NET_OBJ];
}DT_OFP_wifi;

typedef struct
{
	uint16 blink_time_interval_msec;
	uint16 adc_level;
}DT_OFP_gpio;

typedef struct
{
	DT_OFP_wifi ofp_wifi;
	DT_OFP_gpio ofp_gpio;
	bool status;
}DT_OFP_main;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_OFP_framer_init(void);
bool p_OFP_get_params(DT_OFP_main* const xo_ofp_main);
bool p_OFP_clear_params(E_OFP_CLR xi_clr);


#else
	#include "WProgram.h"
#endif


#endif

