
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
	
	xi_file_name = "/" + xi_file_name;

	*xo_file_suffix = p_SFS_get_file_extension(xi_file_name);

	if (true == SPIFFS.exists(xi_file_name))
	{
		*xo_file = SPIFFS.open(xi_file_name, "r");
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

	String file_name = "", file_size = "";
	Dir dir = SPIFFS.openDir("/");
	while (dir.next()) 
	{
		file_name += dir.fileName();
		file_name += " / ";

		file_size += dir.fileSize();
		file_size += "\r\n";
	}
	Serial.print("File name: " + file_name + " File size: " + file_size + "\n\r");
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