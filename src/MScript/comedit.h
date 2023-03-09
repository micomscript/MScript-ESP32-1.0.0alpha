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
 * comedit.h - command editor
 */
#ifndef _COMEDIT_H_
#define _COMEDIT_H_

const int Com_Buff_Size = 128;
const int Hist_Max = 5;

class CommandEditor {
private:
	static char prompt[8];

	int		pos; 		// カーソルの位置
	int		cnt; 		// 文字数
	char	inBuffer[Com_Buff_Size]; // 入力バッファ
	char	histBuff[Hist_Max][Com_Buff_Size]; // 履歴バッファ
	int		histCur;	// 現在の履歴位置
	int		histNext;	// 次の履歴位置

	// 編集機能 ----------------------------------------------------------------
	int	 onKeyIn(char code);
	int  execCommand();        // コマンド入力完了、コマンド実行
	void shiftRight();         // カーソル以降を右へシフト
	void shiftLeft();          // カーソル以降を左へシフト
	void insChar(char code);   // カーソル位置に文字を挿入
	void delChar();            // カーソル上の文字を削除
	void backSpace();          // カーソルの左の文字を削除
	void printRight();         // カーソル以降を表示
	void procESC(char seq[]);  // エスケープシーケンスを処理する
	bool isNormalChar(char c); // 一般の文字（20～7E）
	void printPrompt();        // プロンプトを表示する
	
	// 履歴機能 -----------------------------------------------------------------
	void restoreHist();        // 履歴を復元する（履歴バッファ→入力バッファ）
	void pushHistory();        // 履歴を保存する（入力バッファ→履歴バッファ）
	void clearHistory();       // 履歴をクリアする

public:
	CommandEditor() {
		reset(true);
		clearHistory();
	}

	void reset(bool prompt);
	int  readLine(char *buff, int n, bool input);
};

#endif
