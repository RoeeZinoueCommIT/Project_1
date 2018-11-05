// UTL.h

#ifndef _UTL_h
#define _UTL_h

#if defined(ARDUINO) && ARDUINO >= 100
	
#include "arduino.h"

char* p_UTL_read_str_to_carriage(uint16 max_chars);

#else
	#include "WProgram.h"
#endif


#endif

