#include "TitleScene.h"

/*
@brief	コンストラクタ

@param	　　なし
@return		なし

@note      各種初期設定。クラス生成の際に必要なパラメータはすべて設定する

*/
TitleScene::TitleScene()
{
	mIsOperationGuideScreen = FALSE;
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note      クラス内メンバは自動で解放されるので特に記載なし。
*/
TitleScene::~TitleScene()
{
}
/*
@brief	メインループ内で動作させる更新処理を行う関数

@param[in]	なし
@return     シーンナンバー  :int
@note		タイトルシーン時/    SCENE_TITLE　  0
@note		ゲームメインシーン時/SCENE_GAMEMAIN 1
@note		リザルトシーン時/    SCENE_RESULT   2
		     
@note　　　　スペースキーの押されたかで表示画像とシーン遷移の処理を行う
@warning
- シーン遷移の判定を行うため、返り値をint型にしている
*/
int TitleScene::action()
{
	if (checkSinglePushKey(KEY_INPUT_SPACE))
	{
		if (mIsOperationGuideScreen == FALSE)
		{
			mIsOperationGuideScreen = TRUE;
		}
		else
		{
			return SCENE_GAMEMAIN;
		}
	}
	return SCENE_TITLE;
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note   タイトルと操作説明画像を 画面切り替えフラグに応じて表示
*/
void TitleScene::draw()
{
	if (mIsOperationGuideScreen == FALSE)
	{
		DrawGraph(SCREEN_ORIGIN_POINT, SCREEN_ORIGIN_POINT, Data::getInstance()->mTitleImageHandle, true);
	}
	else
	{
		DrawGraph(SCREEN_ORIGIN_POINT, SCREEN_ORIGIN_POINT, Data::getInstance()->mOperationGuideImageHandle, true);
	}
}

/*
@brief	動作開始時に必要なパラーメータの初期設定や処理を行う関数

@param	なし
@return		なし

@note      他シーンのBGMを止める
@note      タイトルシーンBGMの再生
@note      各パラメータの初期設定

*/
void TitleScene::start()
{
	StopSoundMem(Data::getInstance()->mGameBgmHandle);
	StopSoundMem(Data::getInstance()->mTitleBgmHandle);
	PlaySoundMem(Data::getInstance()->mTitleBgmHandle, DX_PLAYTYPE_LOOP);
	mIsOperationGuideScreen = FALSE;
}

