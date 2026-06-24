#pragma once
#include "DxLib.h"
#include "define.h"
// 最大数
#define MAX_NUMBER_EFFECT (9)
#define MINI_BULLET_TYPE_MAX (2)
#define PLAYER_BULLET_TYPE_MAX (3)


enum BULLET_TYPE_DATA
{
	P_NOMAL = 0,
	P_MISSILE,
	P_SPECIAL,
	E_NOMAL,
	E_MINI_ORANGE,
	E_MINI_YELLOW,
};


class Data
{
  public:
	static void createInstance(); // インスタンスの生成を行うための関数
	static void destroyInstance(); // インスタンスを解放する関数
	static Data *getInstance(); // インスタンスを取得するための関数
	void load();                // データの読み込みを行う関数
	void release(); // リソースの解放処理を行うための関数

	//---リザルトデータ---
	int mResultGameScore;   // ゲームスコア
	int mInGameTimeSecond;  // ゲーム内経過時間(秒)
	int mInGameTimeMinutes; // ゲーム内経過時間(分)
	//--------------------

	//---使用アニメーション---
	int mBlueExplosionHandle;                           // 青爆発アニメ
	int mEffectAnimationHandleArray[MAX_NUMBER_EFFECT]; // エフェクトアニメ
	//------------------------

	//---使用動画---
	int mBackGroundMovieHandle; // 背景動画
	//--------------

	//---使用画像---
	int mTitleImageHandle;          // タイトル画像
	int mGameOverTelopImageHandle;  // ゲームオーバーテロップ画像
	int mRedBackGroundImageHandle;  // 赤背景画像
	int mOperationGuideImageHandle; // 操作説明画像

	int mPleyerImageHandle;           // 通常プレイヤー画像
	int mPleyerDamageImageHandle;     // ダメージ用プレイヤー画像
	int mPleyerUnbeatableImageHandle; // 無敵用プレイヤー画像

	int mBossImageHandle;       // ボス画像
	int mBossDamageImageHandle; // ダメージ用ボス画像

	int mEnemyImageHandle;       // 通常雑魚敵の画像
	int mTraceEnemyImageHandle;  // 追従雑魚敵の画像
	int mChargeEnemyImageHandle; // チャージ雑魚敵の画像
	int mEnemyDamageImageHandle; // 雑魚的共通ダメージ用画像

	int mEnemyBulletImageHandle; // 通常敵弾の画像
	int mEnemyMiniBulletImageHandleArray
	    [MINI_BULLET_TYPE_MAX]; // 敵ミニ弾の画像配列(色違い)
	int mPlayerBulletImageHandleArray
	    [PLAYER_BULLET_TYPE_MAX]; // プレイヤー弾の画像配列

	int mBulletImgageHandleArray[MAX_BULLET_TYPE_NUMBER]; // 弾画像配列

	int mLifeItemImageHandle;             // 回復アイテム画像
	int mExperiencePointsItemImageHandle; // 経験値アイテム画像
	int mUnbeatableItemImageHandle;       // 無敵アイテム画像
	//--------------

	//---ゲーム内使用サウンド---
	int mGameBgmHandle;  // ゲーム本編BGM
	int mTitleBgmHandle; // タイトルBGM

	int mPowerUpSoundEffectHandle;      // パワーアップSE
	int mPlayerDamageSoundEffectHandle; // プレイヤーダメージSE
	int mDefeatEnemySoundEffectHandle;  // 敵撃破SE

	int mEnemyShotSoundEffectHandle;  // 敵発射SE
	int mPlayerShotSoundEffectHandle; // プレイヤー発射SE

	int mEXPItemGetSoundEffectHandle;        // 経験値アイテム獲得SE
	int mLifeItemGetSoundEffectHandle;       // 回復アイテム獲得SE
	int mUnbeatableItemGetSoundEffectHandle; // 無敵アイテム獲得SE
	//--------------------------

  private:
	Data();
	~Data();

	static Data *mInstance; // 唯一のクラスポインタ
};