/*
 * メモ
 *     シリアル通信速度：115200
 *     改行コード：　受信LF  送信CR
 *     ローカルエコー：OFF
 */
#include <stdio.h>
#include <string.h>
#include <FS.h>
#include <SPIFFS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "common.h"
#include "fsystem.h"
#include "expression.h"
#include "logic.h"
#include "sentence.h"
#include "comedit.h"
#include "token.h"
#include "tokenizer.h"
#include "parser.h"
#include "varmap.h"

Parser parser;
extern TFT_eSPI _tft;
extern MSFileSystem	_fileSystem;
int _system_mode;	// 0:Dialog Mode  1:Script Mode

void setup() {
	_system_mode = 0;
	_fileSystem.initFSystem();
	_tft.init();
	_tft.setRotation(0);
	_tft.setTextColor(0x0000,0xffff,false);
	_tft.fillScreen(TFT_WHITE);

	Serial.begin(115200);
	
	Serial.println("\nMicomScript 1.0.0-alpha\n");
//	parser.execute_file("/init");
}

void loop() {
	parser.parse();
}
