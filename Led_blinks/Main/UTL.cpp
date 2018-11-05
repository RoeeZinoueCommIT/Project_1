#include "UTL.h"

char* p_UTL_read_str_to_carriage(uint16 max_chars)
{
	char buffer[20];
	while (!Serial.available());
	Serial.readBytesUntil('\n', buffer, 20);
	return (buffer);
}

