/****************************************************************
* Description: Offline parameters module.
* Module name: OFP
* Version: 1_001
* Date: 29-10-2018
****************************************************************/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include <FS.h>
#include "OFP_framer.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

DT_OFP_main g_OFP_main;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% LOCAL DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup OFP
\brief Interface function:
The function shall return Offline parameters structure. 
\return Bool: True - Global Offline structure can be read well, 
              False - There is at least 1 failure during access to Offline parameters data in SPIFFS. 
\param DT_OFP_main* const xo_ofp_main - Pointer to external Offilne parameters structure. 
		range: Not NULL.
*/
/*--------------------------------------------------------------------------------------------------*/
bool p_OFP_get_offline_params(DT_OFP_main* const xo_ofp_main)
{
	bool ret = false;
	
	if (true == g_OFP_main.status)
	{
		memcpy(xo_ofp_main, &g_OFP_main, sizeof(g_OFP_main));
		ret = true;
	}
	else
	{
		ret = false;
	}
	return (ret);
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup OFP
\brief Interface function:
The function shall clear request fields in Offline parameters structure. 
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
bool p_OFP_clear_params(E_OFP_CLR xi_clr)
{
	switch (xi_clr)
	{
		case E_OFP_CLR_WIFI:
			memset(&g_OFP_main.ofp_wifi, NULL, sizeof(g_OFP_main.ofp_wifi));
			break;

		case E_OFP_CLR_GPIO:
			break;
	}
}

bool p_OFP_save_file_to_memory(File dt)
{

}

