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
 * tokenn.cpp
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "token.h"

//演算子用
Token::Token(int code) {
	_code = code;
	_iValue = 0;
	_fValue = 0;
	_string = NULL;
}

//数値用
Token::Token(int code,long value) {
	_code = code;
	_iValue = value;
	_string = NULL;
}

Token::Token(int code,double value) {
	_code = code;
	_fValue = value;
	_string = NULL;
}

//名前用
Token::Token(int code,char *string) {
	_code = code;
	_string = new char[strlen(string) + 1];
	strcpy(_string,string);
}

Token::~Token() {
#ifdef DEBUG
	printf("delete Token...");
	print();
#endif
	delete [] _string;
}

void Token::clear() {
	delete this;
}

//字句種別コードを返す
int Token::getCode() {
	return _code;
}

//数値を返す（数値の場合）
long Token::getInteger() {
	return _iValue;
}
double Token::getFloat() {
	return _fValue;
}

char *Token::getString() {
	return _string;
}

int Token::getkeyCode() {
	return _keyCode;
}

//表示
void Token::print() {
	printf("  Token : ");
	if(_code==T_EOL)				printf("EOL");
	else if(_code == T_Name)		printf("Name ( %s )",_string);
	else if(_code == T_KeyWord)		printf("Key Word ( %d )",_keyCode);
	else if(_code == T_Integer)		printf("Integer ( %ld )",_iValue);
	else if(_code == T_Float)		printf("Float ( %f )",_fValue);
	else if(_code == T_Text)		printf("Text ( %s )",_string);
	else if(_code == T_ListWord)	printf("List Word ( %d )",_keyCode);
	else if(_code == T_Plus)		printf("+");
	else if(_code == T_Minus)		printf("-");
	else if(_code == T_Aster)		printf("*");
	else if(_code == T_Slash)		printf("/");
	else if(_code == T_Percent)		printf("%");
	else if(_code == T_Aster2)		printf("**");
	else if(_code == T_Plus2)		printf("++");
	else if(_code == T_Minus2)		printf("--");
	else if(_code == T_Equal)		printf("=");
	else if(_code == T_PlusEQ)		printf("+=");
	else if(_code == T_MinusEQ)		printf("-=");
	else if(_code == T_AsterEQ)		printf("*=");
	else if(_code == T_SlashEQ)		printf("/=");
	else if(_code == T_PercentEQ)	printf("%=");
	else if(_code == T_Aster2EQ)	printf("**=");
	else if(_code == T_AndEQ)		printf("&=");
	else if(_code == T_OrEQ)		printf("|=");
	else if(_code == T_ExorEQ)		printf("^=");
	else if(_code == T_TildeEQ)		printf("~=");
	else if(_code == T_LShiftEQ)	printf("<<=");
	else if(_code == T_RShiftEQ)	printf(">>=");
	else if(_code == T_RShift2EQ)	printf(">>>=");
	else if(_code == T_And)			printf("&");
	else if(_code == T_Or)			printf("|");
	else if(_code == T_Exor)		printf("^");
	else if(_code == T_Not)			printf("~");
	else if(_code == T_LShift)		printf("<<");
	else if(_code == T_RShift)		printf(">>");
	else if(_code == T_RShift2)		printf(">>>");
	else if(_code == T_GT)			printf(">");
	else if(_code == T_GE)			printf(">=");
	else if(_code == T_LE)			printf("<=");
	else if(_code == T_LT)			printf("<");
	else if(_code == T_EQ)			printf("==");
	else if(_code == T_NE)			printf("!=");
	else if(_code == T_LogicAnd)	printf("&&");
	else if(_code == T_LogicOr)		printf("||");
	else if(_code == T_LogicNot)	printf("!");
	else if(_code == T_Question)	printf("?");
	else if(_code == T_Colon)		printf(":");
	else if(_code == T_SemiColon)	printf(";");
	else if(_code == T_LPar)		printf("(");
	else if(_code == T_RPar)		printf(")");
	else if(_code == T_LBracket)	printf("[");
	else if(_code == T_RBracket)	printf("]");
	else if(_code == T_LCurly)		printf("{");
	else if(_code == T_RCurly)		printf("}");
	else if(_code == T_Dot)			printf(".");
	else if(_code == T_Comma)		printf(",");
	else if(_code == T_Undef)		printf("Undefined character");
	else							printf("━");
	printf("\n");
}