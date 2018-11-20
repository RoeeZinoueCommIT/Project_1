/****************************************************************
* Brief      
* Module name: 
* Version: 1_001
* Date: 29-10-2018
****************************************************************/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% INCLUDES %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

#include "SFS_framer.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% LOCAL DECLARATIONS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */

static String p_SFS_get_file_extension(String xi_filename);

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% FUNCTIONS IMPLEMENTATION %%%%%%%%%%%%%%%%%%%%%%%%%%% */

void p_SFS_framer_init(void)
{
	bool res = false;

	res = SPIFFS.begin();
	
	if (true == res)
	{
		Serial.println("SFS module init O.K");
	}
	else
	{
		Serial.println("SFS module init FAIL");
	}
}

bool p_SFS_get_file(String xi_file_name, File* xo_file, String* xo_file_suffix)
{
	bool res = false;
	String file_path = "", file_path_gz = "";

	file_path = "/" + xi_file_name;
	file_path_gz = file_path + ".gz";

	*xo_file_suffix = p_SFS_get_file_extension(xi_file_name);
	
	if ((true == SPIFFS.exists(file_path_gz)) || (true == SPIFFS.exists(file_path)))
	{
		if (true == SPIFFS.exists(file_path_gz))
		{	
			file_path += ".gz";
		}
		
		*xo_file = SPIFFS.open(file_path, "r");
		res = true;
	}
	else
	{
		res = false;
	}	            

	return (res);
}

void p_SFS_list_all_files(void)
{
	Serial.println("List all SPIFFS files \n\r");

	String file_name = "";
	size_t file_size = 0x0;
	Dir dir = SPIFFS.openDir("/");
	while (dir.next()) 
	{
		file_name = dir.fileName();
		file_size += dir.fileSize();

		Serial.print("File name: " + file_name + " File size: " + String(file_size) + "\n\r");
	}
	
}

void p_SFS_fomrat(void)
{
	bool res = false;

	res = SPIFFS.format();
	if (true == res)
	{
		Serial.println("Fomrat SPIFFS O.K");
	}
	else if (false == res)
	{
		Serial.println("Fomrat SPIFFS FAIL");
	}
}

String p_SFS_get_file_extension(String xi_filename)
{
	if (xi_filename.endsWith(".htm")) return "text/html";
	else if (xi_filename.endsWith(".html")) return "text/html";
	else if (xi_filename.endsWith(".css")) return "text/css";
	else if (xi_filename.endsWith(".js")) return "application/javascript";
	else if (xi_filename.endsWith(".png")) return "image/png";
	else if (xi_filename.endsWith(".gif")) return "image/gif";
	else if (xi_filename.endsWith(".jpg")) return "image/jpeg";
	else if (xi_filename.endsWith(".ico")) return "image/x-icon";
	else if (xi_filename.endsWith(".xml")) return "text/xml";
	else if (xi_filename.endsWith(".pdf")) return "application/x-pdf";
	else if (xi_filename.endsWith(".zip")) return "application/x-zip";
	else if (xi_filename.endsWith(".gz")) return "application/x-gzip";
	return "text/plain";
}