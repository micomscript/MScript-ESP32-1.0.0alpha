////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Begin license text.

Copyright 2023- MicomScript Project

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
associated documentation files (the ÅgSoftwareÅh), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED ÅgAS ISÅh, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE.

End license text.
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * fsystem.cpp
 */
#include <stdio.h>
#include <FS.h>
#include <SPIFFS.h>
#include "common.h"
#include "fsystem.h"

MSFileSystem	_fileSystem;
extern int _line_number;

void MSFileSystem::initFSystem(){
	if (!SPIFFS.begin(true)){
		printf("Error : Filed to Mount SPIFFS.\n");
		return;
	}
	printf("SPIFFS Mount Succeeded.\n");
	printf("total %8d bytes\n", SPIFFS.totalBytes());
	printf("used  %8d bytes\n", SPIFFS.usedBytes());
}

void MSFileSystem::listFiles(const char *path){
	File dir = SPIFFS.open(path);
	if (!dir){
		printf("Error : Failed to open directory: %s\n", path);
		return;
	}
	if (!dir.isDirectory()){
		printf("%s\n", path);
		return;
	}
//	printf("%s\n", path);
	File file = dir.openNextFile();
	while (file){
		if (file.isDirectory()){
			listFiles(file.name());
		} else {
			printf("%8d %s\n", file.size(), file.name());
		}
		file = dir.openNextFile();
	}
}

void MSFileSystem::saveFile(const char *path){
	File file = SPIFFS.open(path, FILE_WRITE);
	if(!file){
		printf("Error : Failed to open script file (%s)\n", path);
		return;
	}
	int ch, c = ' ';
	int n = 0;
	while (true){
		if(Serial.available()){
			ch = Serial.read();
			if (ch == 0x0d)	// CR
				ch = 0x0a;	// LF
			Serial.write(ch);
//			Serial.printf("%c %02X\n", ch, ch);
			if (c == 0x0a && ch == ';')
				break;
			file.write(ch);
			n++;
			c = ch;
		}
	}
	file.close();
	Serial.printf(" Worte %d character\n", n);
}

void MSFileSystem::showFile(const char *path){
	File file = SPIFFS.open(path);
	if(!file){
		printf("Error : Failed to open script file (%s)\n", path);
		return;
	}
	while (file.available()){
		Serial.write(file.read());
	}
	file.close();
}

void MSFileSystem::removeFile(const char *path){
	if (SPIFFS.exists(path)){
		SPIFFS.remove(path);
	} else {
		printf("Error : Does not exist script file (%s)\n", path);
	}
}
