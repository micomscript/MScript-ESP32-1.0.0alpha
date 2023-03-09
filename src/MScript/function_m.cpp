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

#include <iostream>
#include <math.h>
#include <string.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "common.h"
#include "comedit.h"
#include "expression.h"
#include "logic.h"
#include "varmap.h"
#include "sentence.h"
#include "function.h"
#include "function_m.h"

extern CommandEditor	_command;

TFT_eSPI _tft = TFT_eSPI();

int _gColor = TFT_BLACK;
int _bColor = TFT_WHITE;	//文字の背景色
int textSize = 1;			//文字のサイズ
//------------------------------------------------------
extern VarMapStack _varMap;

/*　Input
Value *Input::evaluate(){
// for ESP32
	char buff[IN_BUFF_SIZE];
	_command.readLine(buff, IN_BUFF_SIZE, false);
	return new Text(buff);
}
*/


Value *SetColor::evaluate(){
	int color,back;
	color = _list -> getElement(0) -> getInt();
	if(_list->size() == 2){
		back = _list -> getElement(1) -> getInt();
		_bColor = back;
	}
	_gColor = color;
	
	_tft.setTextColor(_gColor,_bColor);
	return NULL;
}

Value *TextSize::evaluate(){
	textSize = _list -> getElement(0) -> getInt();
	_tft.setTextSize(textSize);
	return NULL;
}

Value *ClearScreen::evaluate(){
	_tft.fillScreen(TFT_WHITE);
	return NULL;
}

Value *FillScreen::evaluate(){
	_tft.fillScreen(_gColor);
	return NULL;
}

Value *DrawLine::evaluate() {
	int x1,x2,y1,y2;
	x1 = _list-> getElement(0)->getInt();
	y1 = _list-> getElement(1)->getInt();
	x2 = _list-> getElement(2)->getInt();
	y2 = _list-> getElement(3)->getInt();

	_tft.drawLine(x1,y1,x2,y2,_gColor);
	return NULL;
}

Value *DrawPixel::evaluate() {
	int x,y;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	_tft.drawPixel(x,y,_gColor);
	return NULL;
}

Value *DrawString::evaluate() {
	int x,y;
	char *str;
	str = _list->getElement(0)->getText();
	x = _list-> getElement(1)->getInt();
	y = _list-> getElement(2)->getInt();
		
	_tft.drawString(str,x,y);
	return NULL;
}

Value *DrawRect::evaluate() {
	int x,y,w,h;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	w = _list-> getElement(2)->getInt();
	h = _list-> getElement(3)->getInt();
	_tft.drawRect(x,y,w,h,_gColor);
	return NULL;
}

Value *FillRect::evaluate() {
	int x,y,w,h;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	w = _list-> getElement(2)->getInt();
	h = _list-> getElement(3)->getInt();
	_tft.fillRect(x,y,w,h,_gColor);
	return NULL;
}

Value *DrawCircle::evaluate() {
	int x,y,r;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	r = _list-> getElement(2)->getInt();
	_tft.drawCircle(x,y,r,_gColor);
	return NULL;
}

Value *FillCircle::evaluate() {
	int x,y,r;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	r = _list-> getElement(2)->getInt();
	_tft.fillCircle(x,y,r,_gColor);
	return NULL;
}

Value *DrawTriangle::evaluate() {
	int x0,y0,x1,y1,x2,y2;
	x0 = _list-> getElement(0)->getInt();
	y0 = _list-> getElement(1)->getInt();
	x1 = _list-> getElement(2)->getInt();
	y1 = _list-> getElement(3)->getInt();
	x2 = _list-> getElement(4)->getInt();
	y2 = _list-> getElement(5)->getInt();
	_tft.drawTriangle(x0,y0,x1,y1,x2,y2,_gColor);
	return NULL;
}
Value *FillTriangle::evaluate() {
	int x0,y0,x1,y1,x2,y2;
	x0 = _list-> getElement(0)->getInt();
	y0 = _list-> getElement(1)->getInt();
	x1 = _list-> getElement(2)->getInt();
	y1 = _list-> getElement(3)->getInt();
	x2 = _list-> getElement(4)->getInt();
	y2 = _list-> getElement(5)->getInt();
	_tft.fillTriangle(x0,y0,x1,y1,x2,y2,_gColor);
	return NULL;
}
Value *DrawFastVLine::evaluate(){
	int x,y,v;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	v = _list-> getElement(2)->getInt();
	_tft.drawFastVLine(x,y,v,_gColor);
	return NULL;
}

Value *DrawFastHLine::evaluate(){
	int x,y,h;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	h = _list-> getElement(2)->getInt();
	_tft.drawFastHLine(x,y,h,_gColor);
	return NULL;
}
Value *DrawEllipse::evaluate(){
	int x,y,rx,ry;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	rx = _list-> getElement(2)->getInt();
	ry = _list-> getElement(3)->getInt();
	_tft.drawEllipse(x,y,rx,ry,_gColor);
	return NULL;
}
Value *FillEllipse::evaluate(){
	int x,y,rx,ry;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	rx = _list-> getElement(2)->getInt();
	ry = _list-> getElement(3)->getInt();
	_tft.fillEllipse(x,y,rx,ry,_gColor);
	return NULL;
}
Value *DrawRoundRect::evaluate(){
	int x,y,w,h,r;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	w = _list-> getElement(2)->getInt();
	h = _list-> getElement(3)->getInt();
	r = _list-> getElement(4)->getInt();
	_tft.drawRoundRect(x,y,w,h,r,_gColor);
	return NULL;
}
Value *FillRoundRect::evaluate(){
	int x,y,w,h,r;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	w = _list-> getElement(2)->getInt();
	h = _list-> getElement(3)->getInt();
	r = _list-> getElement(4)->getInt();
	_tft.fillRoundRect(x,y,w,h,r,_gColor);
	return NULL;
}
Value *DrawCircleHelper::evaluate(){
	int x,y,r,c;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	r = _list-> getElement(2)->getInt();
	c = _list-> getElement(3)->getInt();
	_tft.drawCircleHelper(x,y,r,c,_gColor);
	return NULL;
}
Value *FillCircleHelper::evaluate(){
	int x,y,r,c,d;
	x = _list-> getElement(0)->getInt();
	y = _list-> getElement(1)->getInt();
	r = _list-> getElement(2)->getInt();
	c = _list-> getElement(3)->getInt();
	d = _list-> getElement(4)->getInt();
	_tft.fillCircleHelper(x,y,r,c,d,_gColor);
	return NULL;
}
//GPIO関数----------------------------------------
Value *PinMode::evaluate(){
	int pin,io;
	pin = _list->getElement(0)->getInt();
	io = _list->getElement(1)->getInt();
	if(io==0){
		pinMode(pin,OUTPUT);
	}else if(io==1){
		pinMode(pin,INPUT);
	}else if(io==2){
		pinMode(pin,INPUT_PULLUP);
	}else{
		printf("エラー\n");
	}
	
	return NULL;
}

Value *DigitalWrite::evaluate(){
	int pin,HL;
	pin = _list-> getElement(0)->getInt();
	HL = _list-> getElement(1)->getInt();
	if(HL == 0){
		digitalWrite(pin,LOW);
	}else if(HL == 1){
		digitalWrite(pin,HIGH);
	}else{
		printf("エラー：HIGHは1、LOWは0\n");
	}
	return NULL;
}

Value *AnalogRead::evaluate(){
	int pin;
	pin = _list-> getElement(0) ->getInt();
	int analogValue = analogRead(pin);
	return new Integer(analogValue);
}

Value *DigitalRead::evaluate(){
	int pin;
	pin = _list-> getElement(0) ->getInt();
	int digitalValue = digitalRead(pin);
	return new Integer(digitalValue);
}
//--------------------------------------------------
/*Value *Buzzer::evaluate(){
	int pin;
	unsigned int tones;
	pin = _list-> getElement(0)->getInt();
	tones = _list-> getElement(1)->getInt();
	ledcSetup(1,tones,16);
	ledcAttachPin(pin,1);
	ledcWrite(1,tones);
	return NULL;
}

Value *StopBuzzer::evaluate(){
	int ch;
	ch = _list-> getElement(0)->getInt();
	ledcWrite(ch,0);
	return NULL;
}*/

Value *LedcSetup::evaluate(){
	int ch,bits;
	unsigned int freq;
	ch = _list-> getElement(0)->getInt();
	freq = _list-> getElement(1)->getInt();
	bits = _list-> getElement(2)->getInt();
	ledcSetup(ch,freq,bits);
	return NULL;
}

Value *LedcDetachPin::evaluate(){
	int pin;
	pin = _list-> getElement(0)->getInt();
	ledcDetachPin(pin);
	return NULL;
}

Value *LedcAttachPin::evaluate(){
	int ch,pin;
	pin = _list-> getElement(0)->getInt();
	ch = _list-> getElement(1)->getInt();
	ledcAttachPin(pin,ch);
	return NULL;
}

Value *LedcWrite::evaluate(){
	int ch;
	unsigned int tones;
	ch = _list-> getElement(0)->getInt();
	tones = _list-> getElement(1)->getInt();
	ledcWrite(ch,tones);
	return NULL;
}

Value *LedcWriteTone::evaluate(){
	int ch;
	unsigned int freq;
	ch = _list-> getElement(0)->getInt();
	freq = _list-> getElement(1)->getInt();
	ledcWriteTone(ch,freq);
	return NULL;
}

Value *LedcRead::evaluate(){
	int ch;
	ch = _list-> getElement(0)->getInt();
	int readDuty = ledcRead(ch);
	return new Integer(readDuty);
}

Value *LedcReadFreq::evaluate(){
	int ch;
	ch = _list-> getElement(0)->getInt();
	int readFreq = ledcReadFreq(ch);
	return new Integer(readFreq);
}
