
#ifndef _SFS_FRAMER_h
#define _SFS_FRAMER_h

#if defined(ARDUINO) && ARDUINO >= 100

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include <FS.h>
#include "arduino.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% PUBLIC DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_SFS_framer_init(void);
bool p_SFS_get_file(String xi_file_name, File* xo_file, String* xo_file_suffix);
void p_SFS_list_all_files(void);
void p_SFS_fomrat(void);

#else
	#include "WProgram.h"
#endif

#endif /* _SFS_FRAMER_h */

