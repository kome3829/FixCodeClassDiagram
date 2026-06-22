#include "deta.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note	各ハンドル変数の初期化を行う。
@note	コンストラクタは private
にしており、外部から直接インスタンス生成できないようにしている。
@note	インスタンスの生成は init
関数経由のみに制限しており、シングルトンとして動作させる設計。
@note
※初期化の書式については特に理由なく作成したため、他ファイルと統一されていない
*/
Data::Data()
{
	//---リザルトデータ---
	mResultGameScore = 0;   // ゲームスコア
	mInGameTimeSecond = 0;  // ゲーム内経過時間(秒)
	mInGameTimeMinutes = 0; // ゲーム内経過時間(分)
	//--------------------

	//---使用アニメーション---
	mBlueExplosionHandle = 0; // 青爆発アニメ
	for (int i = 0; i < MAX_NUMBER_EFFECT; i++)
	{
		mEffectAnimationHandleArray[i] = 0; // エフェクトアニメ
	}
	//------------------------

	//---使用動画---
	mBackGroundMovieHandle = 0; // 背景動画
	//--------------

	//---使用画像---
	mTitleImageHandle = 0;          // タイトル画像
	mGameOverTelopImageHandle = 0;  // ゲームオーバーテロップ画像
	mRedBackGroundImageHandle = 0;  // 赤背景画像
	mOperationGuideImageHandle = 0; // 操作説明画像

	mPleyerImageHandle = 0;           // 通常プレイヤー画像
	mPleyerDamageImageHandle = 0;     // ダメージ用プレイヤー画像
	mPleyerUnbeatableImageHandle = 0; // 無敵用プレイヤー画像

	mBossImageHandle = 0;       // ボス画像
	mBossDamageImageHandle = 0; // ダメージ用ボス画像

	mEnemyImageHandle = 0;       // 通常雑魚敵の画像
	mTraceEnemyImageHandle = 0;  // 追従雑魚敵の画像
	mChargeEnemyImageHandle = 0; // チャージ雑魚敵の画像
	mEnemyDamageImageHandle = 0; // 雑魚的共通ダメージ用画像
	mEnemyBulletImageHandle = 0; // 通常敵弾の画像
	for (int i = 0; i < MINI_BULLET_TYPE_MAX; i++)
	{
		mEnemyMiniBulletImageHandleArray[i] = 0; // 敵ミニ弾の画像配列(色違い)
	}
	for (int i = 0; i < PLAYER_BULLET_TYPE_MAX; i++)
	{
		mPlayerBulletImageHandleArray[i] = 0; // プレイヤー弾の画像配列
	}

	for (int i = 0; i < MAX_BULLET_TYPE_NUMBER; i++)
	{

		mBulletImgageHandleArray[i] = 0;//弾の画像配列
	}

	mLifeItemImageHandle = 0;             // 回復アイテム画像
	mExperiencePointsItemImageHandle = 0; // 経験値アイテム画像
	mUnbeatableItemImageHandle = 0;       // 無敵アイテム画像
	//--------------

	//---ゲーム内使用サウンド---
	mGameBgmHandle = 0;  // ゲーム本編BGM
	mTitleBgmHandle = 0; // タイトルBGM

	mPowerUpSoundEffectHandle = 0;      // パワーアップSE
	mPlayerDamageSoundEffectHandle = 0; // プレイヤーダメージSE
	mDefeatEnemySoundEffectHandle = 0;  // 敵撃破SE

	mEnemyShotSoundEffectHandle = 0;  // 敵発射SE
	mPlayerShotSoundEffectHandle = 0; // プレイヤー発射SE

	mEXPItemGetSoundEffectHandle = 0;        // 経験値アイテム獲得SE
	mLifeItemGetSoundEffectHandle = 0;       // 回復アイテム獲得SE
	mUnbeatableItemGetSoundEffectHandle = 0; // 無敵アイテム獲得SE
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note	各ハンドル変数のリソース解放は Release
関数で行う設計のため、デストラクタ内では特に記載なし。
@note	デストラクタは private
に設定し、外部からインスタンスの破棄ができないようにしている。
@note
インスタンスの生成・破棄を制御することで、シングルトンとして動作させている。
@note	※各ハンドル変数のリソース解放には手動で Release 関数を呼び出す必要がある

*/
Data::~Data()
{
}

/*
@brief
インスタンスの生成を行うための関数。インスタンスが未生成の場合のみ生成する

@param     	なし
@return     なし
@note		インスタンスのポインタを private static
にすることで、プログラム全体で1つのインスタンスを共有するようにしている
@note		コンストラクタが private
のため、外部から直接インスタンスを生成できないように制限している
@note		public static
関数にすることで、オブジェクトを生成しなくても外部から呼び出せるようにしている
*/
void Data::createInstance()
{
	if (mInstance != nullptr)
	{
		return;
	}
	mInstance = new Data();
}

/*
@brief
インスタンスを解放する関数。作成時解放処理をわすれており、使用されていない

@param     	なし
@return     なし
*/
void Data::destroyInstance()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = nullptr;
	}
}

/*
@brief	インスタンスを取得するための関数

@param     	なし
@return[in]    唯一のインスタンスのポインタ: data*
@note		内部で init 関数を呼び出し、インスタンスが未生成の場合は生成を行う
@note		返り値として、本クラスの唯一のインスタンスへのポインタを取得できる
@note		public static
関数のため、オブジェクトを生成しなくても外部から呼び出すことができる
@note
これにより、この関数を介してどこからでも同一のインスタンスにアクセスできる
*/
Data *Data::getInstance()
{
	createInstance();
	return mInstance;
}

/*
@brief	画像・音声・動画データの読み込みを行う関数

@param     	なし
@return     なし
@note
クラス内で保持している各ハンドルに対して、外部ファイルからデータを読み込み、使用可能な状態にする
@note     外部から呼び出す場合は、getInstance の返り値を介してアクセスする
*/
void Data::load()
{
	// 画像データの読み込み
	mRedBackGroundImageHandle = LoadGraph("Images/RB.png");
	mTitleImageHandle = LoadGraph("Images/title.png");
	mGameOverTelopImageHandle = LoadGraph("Images/gameo.png");
	mPleyerImageHandle = LoadGraph("Images/player(1).png");
	mPleyerDamageImageHandle = LoadGraph("Images/player2(1).png");
	mPleyerUnbeatableImageHandle = LoadGraph("Images/player-yallow.png");
	mLifeItemImageHandle = LoadGraph("Images/life.png");
	mUnbeatableItemImageHandle = LoadGraph("Images/star.png");
	mExperiencePointsItemImageHandle = LoadGraph("Images/EXP.png");
	mPlayerBulletImageHandleArray[0] =
	    LoadGraph("Images/ballet.png");
	mPlayerBulletImageHandleArray[1] =
	    LoadGraph("Images/missaile.png");
	mPlayerBulletImageHandleArray[2] =
	    LoadGraph("Images/specielBallet.png");
	mBossImageHandle = LoadGraph("Images/boss.png");
	mBossDamageImageHandle = LoadGraph("Images/bossdmg.png");
	mEnemyImageHandle = LoadGraph("Images/enemy.png");
	mTraceEnemyImageHandle = LoadGraph("Images/enemy2.png");
	mChargeEnemyImageHandle = LoadGraph("Images/enemy3.png");
	mEnemyDamageImageHandle = LoadGraph("Images/ufo_2.png");
	mEnemyBulletImageHandle = LoadGraph("Images/enemyBallet.png");
	mEnemyMiniBulletImageHandleArray[MINI_BULLET_YELLOW] =
	    LoadGraph("Images/enemyBalletmini.png");
	mEnemyMiniBulletImageHandleArray[MINI_BULLET_ORANGE] =
	    LoadGraph("Images/enemyBalletmini2.png");

	mBulletImgageHandleArray[0] = LoadGraph("Images/ballet.png");
	mBulletImgageHandleArray[1] = LoadGraph("Images/missaile.png");
	mBulletImgageHandleArray[2] = LoadGraph("Images/specielBallet.png");
	mBulletImgageHandleArray[3] = LoadGraph("Images/enemyBallet.png");
	mBulletImgageHandleArray[4] =
	    LoadGraph("Images/enemyBalletmini2.png");
	mBulletImgageHandleArray[5] =
	    LoadGraph("Images/enemyBalletmini.png");

	mOperationGuideImageHandle = LoadGraph("Images/explanation.png");

	// Soundデータ読み込み
	mGameBgmHandle = LoadSoundMem("Sounds/maou_bgm_cyber43.mp3");
	mTitleBgmHandle = LoadSoundMem("Sounds/仮ロビー画面BGM.mp3");
	mDefeatEnemySoundEffectHandle = LoadSoundMem("Sounds/ショット命中.mp3");
	mPlayerShotSoundEffectHandle = LoadSoundMem("Sounds/se_beam2.mp3");
	mEXPItemGetSoundEffectHandle = LoadSoundMem("Sounds/カーソル移動3.mp3");
	mLifeItemGetSoundEffectHandle = LoadSoundMem("Sounds/回復魔法1.mp3");
	mUnbeatableItemGetSoundEffectHandle =
	    LoadSoundMem("Sounds/se_powerup_009.mp3");
	mEnemyShotSoundEffectHandle = LoadSoundMem("Sounds/se_shot_001.mp3");
	mPowerUpSoundEffectHandle = LoadSoundMem("Sounds/パワーアップ.mp3");
	mPlayerDamageSoundEffectHandle = LoadSoundMem("Sounds/se_damage_robo.mp3");

	// アニメ―ションデータの読み込み
	mBlueExplosionHandle = LoadGraph("Animations/Effect_p003.png");
	mEffectAnimationHandleArray[POWERUP_EF] =
	    LoadGraph("Animations/Effect_p029.png");
	mEffectAnimationHandleArray[CHARGE_EF] =
	    LoadGraph("Animations/pipo-btleffect107i.png");
	mEffectAnimationHandleArray[HIT_EF] = LoadGraph("Animations/花火透過.png");
	mEffectAnimationHandleArray[SHOT_EF] =
	    LoadGraph("Animations/pipo-mapeffect009.png");
	mEffectAnimationHandleArray[LIFE_EF] =
	    LoadGraph("Animations/pipo-btleffect110d.png");
	mEffectAnimationHandleArray[STAR_EF] =
	    LoadGraph("Animations/pipo-btleffect110j.png");
	mEffectAnimationHandleArray[WARP_EF] = LoadGraph("Animations/warp_320.png");
	mEffectAnimationHandleArray[CHARGE_EF_BOSS] =
	    LoadGraph("Animations/pipo-btleffect107i_2.png");

	// 動画の読み込み
	mBackGroundMovieHandle = LoadGraph("movies/96621_1280x720.mp4");
}

/*
@brief	各ハンドルに対するリソースの解放処理を行うための関数

@param     	なし
@return     なし
@note     load 関数で読み込んだ画像・音声・動画データを解放する役割を持つ
@note     作成時、解放処理を忘れているため記載がなく、使用もしていない
*/
void Data::release()
{
}