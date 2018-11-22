/****************************************************************
* Description: Main ino module.
* Module name: MAIN.
* Version: 1_001
* Date: 29-10-2018
****************************************************************/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */


#include "APP_framer.h"
#include "HKY_framer.h"
#include "WFI_framer.h"
#include "SFS_framer.h"

/* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% GLOBAL VARIABLES %%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

/* Declare reset function @ address 0 */
void(*p_MAIN_reset_func) (void) = 0;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup MAIN
\brief Interface function:
The function init BSP layer
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void setup()
{
	p_APP_framer_init();
	p_HKY_framer_init();
	p_WFI_framer_init();
	p_SFS_framer_init();
	p_HKY_led_toggle(C_HKY_GPIO_LED_BLUE, 2);
}

/*$PROCEDURE$---------------------------------------------------------------------------------------*/
/*! \ingroup MAIN
\brief Interface function:
The function shall manage BSP layer main loop.
\return void
\param void.
*/
/*--------------------------------------------------------------------------------------------------*/
void loop()
{
	APP_ERR ret_val = p_APP_cyclic_main(APP_CYCLE_CONTINUE);

	switch (ret_val)
	{
		case APP_ERR_EXIT_PROG:
			Serial.end();
			while (true);
			break;

		case APP_ERR_REST_PROG:
			p_MAIN_reset_func();
			break;
	}
}
