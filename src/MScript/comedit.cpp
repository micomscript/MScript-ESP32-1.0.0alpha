////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Begin license text.

Copyright 2023- MicomScript Project

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
associated documentation files (the “Software”), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
IN THE SOFTWARE.

End license text.
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * comedit.cpp
 */
#include <arduino.h>
#include <string.h>
#include "comedit.h"

char _cursorLeft[] = { 0x1B, 0x5B, 0x44, 0x00 };
char _cursorRight[] = { 0x1B, 0x5B, 0x43, 0x00 };
char _cursorUp[] = { 0x1B, 0x5B, 0x41, 0x00 };
char _cursorDown[] = { 0x1B, 0x5B, 0x42, 0x00 };
char _home[] = { 0x1B, 0x5B, 0x31, 0x7E, 0x00 };
char _end[] = { 0x1B, 0x5B, 0x34, 0x7E, 0x00 };

char CommandEditor::prompt[] = "$ ";
extern int _sentence_depth;

int CommandEditor::readLine(char *buff, int n, bool prompt){
	char ch;
	reset(prompt);
	while (true){
		if(Serial.available()){
			ch = Serial.read();
			onKeyIn(ch);
			if (ch == '\r')
				break;
		}
	}
	pushHistory();
	Serial.println();
	strncpy(buff, inBuffer, n);
	return strlen(buff);
}

void CommandEditor::printPrompt(){
//	Serial.print("\x0d");
//	Serial.print("\x1b[K");
	if (_sentence_depth >= 1){
		Serial.print("  ");
//		for(int i = 1; i < _sentence_depth ; i++){
//			Serial.print("    ");
//		}
	} else {
		Serial.print(prompt);
	}
}
void CommandEditor::reset(bool prompt){
	cnt = 0;
	pos = 0;
	inBuffer[0] = '\0';
	if (prompt)
		printPrompt();
}

/*
 * 一般の文字(20～7E)ならばtrueを返す。
 */
bool CommandEditor::isNormalChar(char c){
	return (c >= 0x20) && (c <= 0x7e);
}

/*
 * pos から行末までを右へシフトする
 */
void CommandEditor::shiftRight(){
	int i;
	for(i = cnt; i >= pos; i--){
		inBuffer[i+1] = inBuffer[i];
	}
}
/*
 * pos から行末までを左へシフトする
 */
void CommandEditor::shiftLeft(){
	int i;
	for(i = pos; inBuffer[i-1] = inBuffer[i] ; i++){
	}
}
/*
 * pos から行末までを表示
 */
void CommandEditor::printRight(){
	int i, n = 0;
	for(i = pos; inBuffer[i] != '\0'; i++){
		Serial.print(inBuffer[i]);
		n++;
	}
	for(i = 0; i < n; i++)
		Serial.print('\b'); // BackSpace
}

void CommandEditor::insChar(char code){
	if (cnt >= Com_Buff_Size - 1)
		return;
	shiftRight();
	inBuffer[pos] = code;
	printRight();
	Serial.print(inBuffer[pos++]);
	cnt++;
}

void CommandEditor::delChar(){
	if (inBuffer[pos] == '\0')
		return;
	pos++;
	shiftLeft();
	pos--;
	Serial.print("\x1b[K");
	printRight();
	cnt--;
}

void CommandEditor::backSpace(){
	if (pos == 0)
		return;
	shiftLeft();
	pos--;
	Serial.print('\b');
	Serial.print("\x1b[K");
	printRight();
	cnt--;
}

int CommandEditor::execCommand(){
	if (cnt == 0)
		return 0;
	Serial.println();
	Serial.println(inBuffer);
	pushHistory();
	reset(true);
	return 1;
}
/*
 * 履歴バッファをクリアする
 */
void CommandEditor::clearHistory(){
	int i;
	for(i = 0; i < Hist_Max; i++)
		histBuff[i][0] = '\0';
	histCur = histNext = 0;
}

/*
 * 入力されたコマンドを履歴バッファへコピーする
 */
void CommandEditor::pushHistory(){
	strcpy(histBuff[histNext], inBuffer);
	histNext = (histNext == Hist_Max - 1) ? 0 : histNext + 1;
	histCur = histNext;
}

/*
 * 履歴バッファを入力領域へコピーする
 */
void CommandEditor::restoreHist(){
	strcpy(inBuffer, histBuff[histCur]);
	Serial.print("\x0d");
	Serial.print("\x1b[K");
	if (_sentence_depth >= 1){
		Serial.print("  ");
	} else {
		Serial.print(prompt);
	}
	cnt = 0;
	for(pos = 0; inBuffer[pos] ; pos++){
		Serial.print(inBuffer[pos]);
		cnt++;
	}
}

/*
 * エスケープシーケンスを処理する
 */
void CommandEditor::procESC(char seq[]){
	if (strcmp(seq, _cursorLeft) == 0){ // ←
		if (pos >= 1){
			Serial.print('\b');
			pos--;
		}
	} else if (strcmp(seq, _cursorRight) == 0){ // →
		if (pos < Com_Buff_Size - 1){
			if (inBuffer[pos] == '\0')
				return;
			Serial.print(inBuffer[pos++]);
		}
	} else if (strcmp(seq, _cursorUp) == 0){ // ↑
		histCur = (histCur == 0) ? Hist_Max - 1 : histCur - 1;
		restoreHist();
	} else if (strcmp(seq, _cursorDown) == 0){ // ↓
		histCur = (histCur == Hist_Max - 1) ? 0 : histCur + 1;
		restoreHist();
	} else if (strcmp(seq, _home) == 0){ // Home
		Serial.print("\x0d");
		if (_sentence_depth >= 1){
			Serial.print("  ");
		} else {
			Serial.print(prompt);
		}
		pos = 0;
	} else if (strcmp(seq, _end) == 0){ // End
		for( ; inBuffer[pos] != '\0'; pos++){
			Serial.print(inBuffer[pos]);
		}
	}
}

int CommandEditor::onKeyIn(char code){
	// エスケープシーケンス
	if (code == 0x1b){
		char buff[8];
		int i = 0;
		buff[i++] = 0x1b;
		while(Serial.available())
			buff[i++] = Serial.read();
		buff[i] = '\0';
		procESC(buff);
		return 0;
	}
	// 通常の文字
	if(isNormalChar(code)){
		insChar(code);
		return 0;
	}
	if (code == 0x0d){ // 改行
//		return execCommand();
		return 0;
	}
	if (code == 0x08){ // BS(BackSpace)
		backSpace();
		return 0;
	}
	if (code == 0x7f){ // DEL
		delChar();
		return 0;
	}
	if (code == 0x09){ // TAB
		int n = 4 - pos % 4;
		int i;
		for(i = 0; i < n; i++){
			insChar(' ');
		}
	}
	// 残りの文字は無視
	return 0;
}
