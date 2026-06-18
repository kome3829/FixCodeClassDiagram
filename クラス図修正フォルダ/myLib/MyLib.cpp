
#include "MyLib.h"
int gFontSize = DEFAULT_SET_FONT_SIZE;

/*
	@brief	 文字列の中央を指定位置して表示させる関数

    @param[in] int drawCentarPositionX    : 文字列の表示のX座標
	@param[in] int drawCentarPositionY    :  文字列の表示のY座標
	@param[in] char* string   : 表示する文字列
	@param[in] int color    : 表示する文字列のカラーコード

	@return		なし

	@note	文字列の長さと幅から計算し、文字列の表示位置を変更し表示している
	@note	DXライブラリの文字列表示関数は文字列の先頭を位置指定しているため作成した

*/
void drawCenterString(int drawCentarPositionX, int drawCentarPositionY,  char* string, int color)
{
	int stringLength;
	int stringWidth;
	stringLength = strlen(string);//文字列の長さの取得
	stringWidth = GetDrawStringWidth(string, stringLength);//文字列の幅を取得
	DrawString(drawCentarPositionX - HALF(stringWidth), drawCentarPositionY - HALF(gFontSize), string, color);
}

/*
	@brief	 表示する文字列のフォントサイズを変更する関数

	@param[in] int Fsize  : 変更するフォントサイズ

	@return		なし

	@note	引数でフォントサイズを設定する
*/
void setFontSize(int fontSize)
{
	gFontSize = fontSize;//フォントサイズをついでで覚える
	SetFontSize(fontSize);
}



//爆破アニメーション
//使用画像
//int explosionAnime = 0;
//int anime_w = 128;
//int anime_h = 128;
//int animecount = 0;
//explosionAnime = LoadGraph("mylib/animation/icon.png");//1枚128*128pxが５*3枚
//void explosionAnimation(int x, int y)
//{
//	int animeTable[21] = { 0,1,2.3,4,5,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14, };
//	int no = animeTable[animecount];
//	int dpx = no % 5; int dpy = no / 5;
//	// グラフィック中の(0,0)-(anime_w,anime_h)の部分を画面座標(320- anime_w/2,240- anime_h/2)に
//	DrawRectGraph(x, y, anime_w * dpx, anime_h * dpy, anime_w, anime_h, explosionAnime, false, FALSE);
//	animecount++;
//	if (animecount > 22)animecount = 22;
//}


//ランダム
//#include<random>
//random_device rnd;
//static mt19937 mt32(rnd());
//unsigned int random = mt32();
//random = random % 186 + 1;