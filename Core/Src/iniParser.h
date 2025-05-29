/*
 * iniParser.h
 *
 *  Created on: May 29, 2025
 *      Author: Sentry
 */

#ifndef SRC_INIPARSER_H_
#define SRC_INIPARSER_H_

#include <string>
#include <map>
#include "FatFS/ff.h"
#include "FatFS/diskio.h"

using INIMap = std::map<std::string, std::map<std::string, std::string>>;

class iniParser {
private:
	FILE* datei;
	FIL File;

public:
	INIMap iniData;
	iniParser() {};

	uint32_t parseINI(std::string filepath){
		std::string path = filepath + "config.ini";
		uint8_t fileBuffer[256];
		UINT readBytes;

	    size_t start = 0;
		FRESULT res;
	    res = f_open(&File, path.c_str(), FA_READ);
		f_read(&File, &fileBuffer[0], 256, &readBytes);
		std::string fileContent(reinterpret_cast<const char*>(fileBuffer), readBytes);


		std::string actualLine, acutalSection;

		while (start < fileContent.size()) {
			size_t end = fileContent.find('\n', start);
			if (end == std::string::npos) {
			    end = fileContent.size(); // letzte Zeile ohne Newline
			    }

			actualLine = fileContent.substr(start, end - start); // aktuelle Zeile kopieren

			// Erase Space, tab, return, new line
		    actualLine.erase(0, actualLine.find_first_not_of(" \t\r\n")); // in front of Line
		    actualLine.erase(actualLine.find_last_not_of(" \t\r\n") + 1); // at the End of Line

		    // Skip empty line and comments
		    if (!(actualLine.empty() || actualLine[0] == ';' || actualLine[0] == '#')) {

		    	if (actualLine.front() == '[' && actualLine.back() == ']') {
		    		acutalSection = actualLine.substr(1, actualLine.size() - 2);
		        }
		        else {
		            std::string key = actualLine.substr(0, actualLine.find_first_of('='));
		            key.erase(key.find_last_not_of(" \t\r\n") + 1); // at the End of Line

		            std::string value = actualLine.substr(actualLine.find_first_of('=') + 1);
		            value.erase(0, value.find_first_not_of(" \t\r\n")); // at the End of Line

		            this->iniData[acutalSection][key] = value;
		        }
		    }

	        start = end + 1; // zum nächsten Zeilenanfang springen
		}
	    return HAL_GetTick();
	}

};


#endif /* SRC_INIPARSER_H_ */
