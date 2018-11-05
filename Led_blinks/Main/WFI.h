// WFI.h

#ifndef _WFI_h
#define _WFI_h

#if defined(ARDUINO) && ARDUINO >= 100
	
#include "arduino.h"

void p_WFI_init(void);
void p_WFI_print_test(void);
void p_WFI_connect(void);
#else
	#include "WProgram.h"
#endif


#endif

