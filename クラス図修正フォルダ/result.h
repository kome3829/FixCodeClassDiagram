#pragma once
#include "BasicScene.h"
#include"define.h"
#include"Deta.h"
#define RED_COLOR			(0xff0000)//タイトルテキストのカラーコード
#define WHITE_COLOR         (0xffffff)//操作説明テキストのカラーコード
#define GRAY_COLOR			(0x7d7d7d)//テキストウィンドウのカラーコード
#define RESULT_WINDOW_LEFT_X             (96)	//ウィンドウボックスの左側座標
#define RESULT_WINDOW_TOP_Y              (72)	//ウィンドウボックスの上端
#define RESULT_WINDOW_WIDTH              (448)	//ウィンドウボックスの横幅
#define RESULT_WINDOW_HEIGHT             (336)  //ウィンドウボックスの縦幅


#define RESULT_TEXT_CENTER_X              (320)//タイトル文字表示の中心ｘ座標
#define RESULT_TITLE_Y_OFFSET             (20)//タイトル文字表示のｙ座標オフセット

#define RESULT_SCORE_X_OFFSET_LEFT		  (143)   //リザルトスコア文字表示位置の左端オフセット
#define RESULT_SCORE_Y_MARGIN             (112)	  //スコア文字表示のウィンドウボックスの上端空の余白

#define RESULT_TIME_X_OFFSET_LEFT        (140)	//リザルトタイム文字表示位置のX座標オフセット
#define RESULT_TIME_Y_OFFSET             (202)  //リザルトタイム文字表示位置のY座標オフセット
//桁
#define RESULT_INITIAL_DIGIT              (1)	//桁数の初期値
#define RESULT_DIGIT_BASE_VALUE         (10)
#define RESULT_SCORE_DIGIT_MAX_VALUE     (9)

//カウント
#define RESULT_UPDATE_INTERVAL_FRAME		 (3)
#define RESULT_TIME_ANIME_START_FRAME        (30)
#define RESULT_TIME_ANIME_RESET_FRAME        (60)

//フォントサイズ
#define RESULT_SCORE_FONT_SIZE            (55)
#define RESULT_TITLE_FONT_SIZE           (45)
#define RESULT_DEFAULT_FONT_SIZE         (16)
#define RESULT_SCORE_TITLE_FONT_SIZE        (45)
#define RESULT_TIME_TITLE_FONT_SIZE          (45)
#define RESULT_DEFAULT_FONT_SIZE             (16)
#define STRING_SPACE_COUNT (2)
//テキスト指定位置
#define RESULT_TITLE_X_OFFSET           (140)
#define RESULT_GUIDE_TEXT_Y_OFFSET       (80)
#define RESULT_SCORE_TITLE_X_OFFSET        (130)
#define RESULT_SCORE_TITLE_Y_OFFSET         (50)
#define RESULT_SCORE_DIGIT_SPACING_DIVISOR (1.5f)
#define RESULT_TIME_TITLE_X_OFFSET          (100)
#define RESULT_TIME_TITLE_Y_OFFSET          (150)
#define RESULT_TIME_ZERO_MINUTE_X_OFFSET_1   (15)
#define RESULT_TIME_ZERO_MINUTE_X_OFFSET_2   (-9)
#define RESULT_TIME_COLON_X_OFFSET           (19)
#define RESULT_TIME_ZERO_SECOND_X_OFFSET_1   (47)
#define RESULT_TIME_ZERO_SECOND_X_OFFSET_2   (80)
#define DRAW_RISULT_STRING_Y  (RESULT_WINDOW_TOP_Y + RESULT_TITLE_Y_OFFSET)//タイトル文字表示のｙ座標
#define DRAW_SCORE_STRING_X  (RESULT_TEXT_CENTER_X - RESULT_SCORE_X_OFFSET_LEFT)//スコア文字表示ｘ座標
#define DRAW_SCORE_STRING_Y  (RESULT_WINDOW_TOP_Y + RESULT_SCORE_Y_MARGIN)//スコア文字表示y座標
#define DRAW_TIME_STRING_X  (RESULT_TEXT_CENTER_X - RESULT_TIME_X_OFFSET_LEFT)//タイム文字表示ｘ座標
#define DRAW_TIME_STRING_Y  (DRAW_RISULT_STRING_Y + RESULT_TIME_Y_OFFSET)//タイム文字表示y座標

class ResultScene : public BasicScene
{
public:
	ResultScene();
	~ResultScene();
	void start();
	int action();
	void draw();
	void drawScoreAnimation();	//スコア表示のアニメーション関数
	void drawTimeAnimation();	//タイム表示のアニメーション関数

	int mResultScore;		//リザルトスコア
	int mResultTimeSeconds;	//リザルトタイム(秒)
	int mResultTimeMinutes;	//リザルトタイム(分)


private:
	int mSceneType;							//シーンの種類
	int mDrawScoreStringSize;				//スコア文字表示サイズ。修正案。変数を削除して、代入している値をそのまま使用
	int mShowScoreArray[SCPRE_MAX_DIGIT];	//表示スコアの各桁の配列。修正案スコアの桁数の増加
	int mShowScoreDigit;					//桁数指定用変数
	bool mIsFinishCountUpProcess;			//各桁の値が０の場合のカウントアップ処理の実行終了判定
	int mUpdateCount;						//カウントアップ処理の更新カウント
	int mDrawTimeUpdateCount;				//点滅表示の更新カウント

};

