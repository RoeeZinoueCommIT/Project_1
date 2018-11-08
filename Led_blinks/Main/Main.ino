

#include "UTL.h"
#include "HKY_framer.h"
#include "WFI.h"
#include "APP_framer.h"

/* Declare reset function @ address 0 */
void(*p_MAIN_reset_func) (void) = 0; 

void setup()
{
	p_APP_framer_init();
	p_HKY_framer_init();
	p_WFI_init();
}

void loop()
{
	switch (p_APP_cyclic_main(APP_CYCLE_CONTINUE))
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