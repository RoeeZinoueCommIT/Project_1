
#ifndef _WFI_h
#define _WFI_h

#if defined(ARDUINO) && ARDUINO >= 100

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "arduino.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_WFI_init(void);
void p_WFI_print_test(void);
void p_WFI_connect(void);
void p_WFI_read_status(void);
void p_WFI_disconnect(void);
void p_WFI_start_ap(void);
void p_WFI_listen_http_client(void);

#else
	#include "WProgram.h"
#endif


#endif

