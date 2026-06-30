#include "Result.h"
/*
@brief	コンストラクタ

@param	　　なし
@return		なし

@note      各種初期設定。クラス生成の際に必要なパラメータはすべて設定する

*/
ResultScene::ResultScene()
{
	mSceneType = SCENE_RESULT;
	mResultScore = Data::getInstance()->mResultGameScore;			//リザルトスコア
	mResultTimeSeconds = Data::getInstance()->mInGameTimeSecond;	//リザルトタイム秒
	mResultTimeMinutes = Data::getInstance()->mInGameTimeMinutes;	//リザルトタイム分
	mDrawScoreStringSize = RESULT_SCORE_FONT_SIZE;
	mShowScoreDigit = RESULT_INITIAL_DIGIT;
	mIsFinishCountUpProcess = false;
	mUpdateCount = 0;
	mDrawTimeUpdateCount = 0;
	for (int i = 0; i < SCPRE_MAX_DIGIT; i++)
	{
		mShowScoreArray[i] = 0;
	}
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note      クラス内メンバは自動で解放されるので特に記載なし。
*/
ResultScene::~ResultScene()
{
}

/*
@brief	動作開始時に必要なパラーメータの初期設定や処理を行う関数

@param	なし
@return		なし

@note      ゲームメインBGMの停止
@note      リザルトSEの再生
@note      各パラメータの初期設定
@note      リザルトスコアをデータクラスから読み込み

*/
void ResultScene::start()
{
	mSceneType = SCENE_RESULT;
	mResultScore = Data::getInstance()->mResultGameScore;			//リザルトスコア
	mResultTimeSeconds = Data::getInstance()->mInGameTimeSecond;	//リザルトタイム秒
	mResultTimeMinutes = Data::getInstance()->mInGameTimeMinutes;	//リザルトタイム分
	mDrawScoreStringSize = RESULT_SCORE_FONT_SIZE;
	mShowScoreDigit = RESULT_INITIAL_DIGIT;
	mIsFinishCountUpProcess = false;
	mUpdateCount = 0;
	mDrawTimeUpdateCount = 0;

	for (int i = 0; i < SCPRE_MAX_DIGIT; i++)
	{
		mShowScoreArray[i] = 0;
	}
}

/*
@brief	メインループで実行する更新処理を行う関数

@param[in]	なし
@return     シーンナンバー  :int
@note		タイトルシーン時/    SCENE_TITLE　  0
@note		ゲームメインシーン時/SCENE_GAMEMAIN 1
@note		リザルトシーン時/    SCENE_RESULT   2
		
@note

- シーン遷移の判定を行うため、返り値をint型にしている
- リザルトスコアのカウントアップ表示を制御を行う
	- mUpdateCount変数を用いてタイミングや動作を制御している
	- 各桁の表示値は、対応するリザルトスコアの値に一致するまで加算する
	- 目標の桁が0の場合は、桁を一巡（9まで）させてから0に到達させる
- Rキー押されたらゲームシーン、Tキー押されたらタイトルシーンへ遷移
@warning
- 目標の桁が0でもカウントアップさせることで動きに統一感をもたせる
*/
int ResultScene::action()
{
	mSceneType = SCENE_RESULT;
	mUpdateCount++;//カウントの更新
	if (mUpdateCount == RESULT_UPDATE_INTERVAL_FRAME)//UpdateCount3フレームに一回処理する
	{
		mUpdateCount = 0;//カウントのリセット
		//リザルトスコアのカウントアップ計算
		//各桁ごとに計算
		for (int i = 0; i < SCPRE_MAX_DIGIT; i++)
		{
			//桁の移動
			for (int k = 0; k < i; k++)//桁の分１０を掛ける
			{
				mShowScoreDigit *= RESULT_DIGIT_BASE_VALUE;
			}
			if (mShowScoreArray[i] < (mResultScore / mShowScoreDigit) % RESULT_DIGIT_BASE_VALUE)//桁の値が0の場合は実行されない
			{
				mShowScoreArray[i]++;//リザルトのスコアになるまで各桁を足し続ける
			}
			//桁数のリセット
			mShowScoreDigit = RESULT_INITIAL_DIGIT;
		}
		//桁の値が0の場合
		if (!mIsFinishCountUpProcess)//各桁のカウントアップ処理が終っていない場合
		{
			mIsFinishCountUpProcess = true;//表示の桁の値が１０になるとtrueになる
			for (int i = 0; i < SCPRE_MAX_DIGIT; i++)
			{
				for (int k = 0; k < i; k++)//桁の分１０を掛ける
				{
					mShowScoreDigit *= RESULT_DIGIT_BASE_VALUE;
				}
				//桁の値が0で9までスコアアップ処理が行われていない場合
				if (mResultScore / mShowScoreDigit % RESULT_DIGIT_BASE_VALUE == 0 &&
					mShowScoreArray[i] < RESULT_SCORE_DIGIT_MAX_VALUE)
				{
					mShowScoreArray[i]++;//９なるまで各桁を足し続ける
					mIsFinishCountUpProcess = false;
				}
				//桁数のリセット
				mShowScoreDigit = RESULT_INITIAL_DIGIT;
			}
		}
		else//各桁のカウントアップ処理が終割った場合
		{
			for (int i = 0; i < SCPRE_MAX_DIGIT; i++)
			{
				//桁の移動
				for (int k = 0; k < i; k++)//桁分１０を掛ける
				{
					mShowScoreDigit *= RESULT_DIGIT_BASE_VALUE;
				}

				//表示の値が10の場合、正しい表示の０にする
				if (mResultScore / mShowScoreDigit % RESULT_DIGIT_BASE_VALUE == 0)
				{
					mShowScoreArray[i] = 0;
				}
				mShowScoreDigit = RESULT_INITIAL_DIGIT;//桁のリセット
			}
		}
	}
	//ゲームシーンへ
	if (checkSinglePushKey(KEY_INPUT_R))
	{
		//修正案。returnを変数に代入せずそのまま定数で返す
		mSceneType = SCENE_GAMEMAIN;
		return mSceneType;
	}
	//タイトルシーンへ
	if (checkSinglePushKey(KEY_INPUT_T))
	{
		//修正案。returnを変数に代入せずそのまま定数で返す
		mSceneType = SCENE_TITLE;
		return mSceneType;
	}
	return mSceneType;
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note      各テキストを表示させるウィンドウボックスを表示
@note      リザルトスコア、リザルトタイムの表示
@note      入力説明のテキストを表示

*/
void ResultScene::draw()
{
	//リザルトウィンドウボックスの表示
	DrawBox(
		RESULT_WINDOW_LEFT_X ,
		RESULT_WINDOW_TOP_Y,
		RESULT_WINDOW_WIDTH + RESULT_WINDOW_LEFT_X,
		RESULT_WINDOW_TOP_Y + RESULT_WINDOW_HEIGHT,
		GRAY_COLOR,
		true
	);
	//リザルトテキストの表示
	SetFontSize(RESULT_TITLE_FONT_SIZE);
	DrawString(
		RESULT_TEXT_CENTER_X - RESULT_TITLE_X_OFFSET,
		DRAW_RISULT_STRING_Y,
		"ＲＥＳＵＬＴ",
		RED_COLOR
	);

	SetFontSize(RESULT_DEFAULT_FONT_SIZE);
	//スコア、タイムの表示
	drawScoreAnimation();
	drawTimeAnimation();
	//操作テキストの表示
	DrawString(
		DRAW_TIME_STRING_X,
		DRAW_TIME_STRING_Y + RESULT_GUIDE_TEXT_Y_OFFSET,
		"リトライ：R　　　　タイトル：T",
		WHITE_COLOR
	);
}

/*
@brief	スコアのカウントアップアニメーションの表示を行う関数

@param	なし
@return		なし

@note action関数で更新された表示値を、各桁ごとに描画する
*/
void ResultScene::drawScoreAnimation()
{
	SetFontSize(RESULT_SCORE_TITLE_FONT_SIZE);
	//テキスト表示
	DrawString(
		RESULT_TEXT_CENTER_X - RESULT_SCORE_TITLE_X_OFFSET,
		DRAW_RISULT_STRING_Y + RESULT_SCORE_TITLE_Y_OFFSET,
		"ＳＣＯＲＥ",
		RED_COLOR
	);
	SetFontSize(mDrawScoreStringSize);
	//スコアを各桁ごとに表示
	for (int i = 0; i < SCPRE_MAX_DIGIT; i++)
	{
		DrawFormatString(
			DRAW_SCORE_STRING_X +
			(int)(mDrawScoreStringSize / RESULT_SCORE_DIGIT_SPACING_DIVISOR) *
			(SCPRE_MAX_DIGIT - i),
			DRAW_SCORE_STRING_Y,
			WHITE_COLOR,
			"%d",
			mShowScoreArray[i]
		);
	}
}

/*
@brief	タイムの点滅表示を行う関数

@param	なし
@return		なし

@note      リザルトタイムの分と秒を表示する
@note      表示値が１桁の場合、２桁目を０表示するように切り替えている
@note      mDrawTimeUpdateCount変数を用いてタイミングや動作を制御している
@warning
- 表示に関するカウント更新処理なのでaction関数に分けずに記載した
*/
void ResultScene::drawTimeAnimation()
{
	mDrawTimeUpdateCount++;//表示カウントの更新

	SetFontSize(RESULT_TIME_TITLE_FONT_SIZE);
	//テキスト表示
	DrawString(
		RESULT_TEXT_CENTER_X - RESULT_TIME_TITLE_X_OFFSET,
		DRAW_RISULT_STRING_Y + RESULT_TIME_TITLE_Y_OFFSET,
		"ＴＩＭＥ",
		RED_COLOR
	);
	SetFontSize(mDrawScoreStringSize);

	//タイム表示カウントが30フレーム以上で表示、点滅表示を行っている。
	if (mDrawTimeUpdateCount >= RESULT_TIME_ANIME_START_FRAME)
	{
		//分の表示
		if (mResultTimeMinutes <= RESULT_SCORE_DIGIT_MAX_VALUE)//10以下ならば2桁目に０を表示
		{
			//0を表示
			DrawString(
				DRAW_TIME_STRING_X + mDrawScoreStringSize +
				RESULT_TIME_ZERO_MINUTE_X_OFFSET_1,
				DRAW_TIME_STRING_Y,
				"0",
				WHITE_COLOR
			);
			//リザルトタイム(分)を表示
			DrawFormatString(
				DRAW_TIME_STRING_X + mDrawScoreStringSize * STRING_SPACE_COUNT +
				RESULT_TIME_ZERO_MINUTE_X_OFFSET_2,
				DRAW_TIME_STRING_Y,
				WHITE_COLOR,
				"%d",
				mResultTimeMinutes
			);
		}
		else
		{
			//リザルトタイム(分)を表示
			DrawFormatString(
				DRAW_TIME_STRING_X + mDrawScoreStringSize,
				DRAW_TIME_STRING_Y,
				WHITE_COLOR,
				"%d",
				mResultTimeMinutes
			);
		}
		//分と秒の間の:を表示
		DrawString(
			DRAW_TIME_STRING_X + mDrawScoreStringSize * STRING_SPACE_COUNT +
			RESULT_TIME_COLON_X_OFFSET,
			DRAW_TIME_STRING_Y,
			":",
			WHITE_COLOR
		);
		//秒の表示
		if (mResultTimeSeconds < RESULT_SCORE_DIGIT_MAX_VALUE)//10以下ならば2桁目に０を表示
		{
			//0を表示
			DrawString(
				DRAW_TIME_STRING_X + mDrawScoreStringSize * STRING_SPACE_COUNT +
				RESULT_TIME_ZERO_SECOND_X_OFFSET_1,
				DRAW_TIME_STRING_Y,
				"0",
				WHITE_COLOR
			);
			//ザルトタイム(秒)を表示
			DrawFormatString(
				DRAW_TIME_STRING_X + mDrawScoreStringSize * STRING_SPACE_COUNT +
				RESULT_TIME_ZERO_SECOND_X_OFFSET_2,
				DRAW_TIME_STRING_Y,
				WHITE_COLOR,
				"%d",
				mResultTimeSeconds
			);
		}
		else
		{
			//ザルトタイム(秒)を表示
			DrawFormatString(
				DRAW_TIME_STRING_X +
				RESULT_TIME_COLON_X_OFFSET +
				RESULT_TIME_COLON_X_OFFSET +
				mDrawScoreStringSize * STRING_SPACE_COUNT,
				DRAW_TIME_STRING_Y,
				WHITE_COLOR,
				"%d",
				mResultTimeSeconds
			);
		}
		if (mDrawTimeUpdateCount >= RESULT_TIME_ANIME_RESET_FRAME)//60フレームでカウントをリセット
		{
			mDrawTimeUpdateCount = 0;
		}
	}
	SetFontSize(RESULT_DEFAULT_FONT_SIZE);
}