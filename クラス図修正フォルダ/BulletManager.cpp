#include "BulletManager.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note      バレットクラスのインスタンスを生成している

*/
BulletManager::BulletManager()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mBullets[i] = new Bullet();
	}
}
/*
@brief	デストラクタ

@param	なし
@return		なし

@note
インスタンス生成した バレットクラスのdeleteを行い、メモリの開放を行う
*/
BulletManager::~BulletManager()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		delete mBullets[i];
		mBullets[i] = nullptr;
	}
}
/*
@brief	メインループで実行する更新処理を行う関数

@param     	なし
@return     なし
@note		生成した全てのバレットクラスの更新処理を行う
*/
void BulletManager::action()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mBullets[i]->action();
	}
}
/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note	生成した全てのバレットクラスの描画処理を行う

*/
void BulletManager::draw()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mBullets[i]->draw();
	}
}
/*
@brief	処理開始に必要なパラーメータの初期設定や処理を行う関数

@param	なし
@return		なし
@note	生成した全てのバレットクラスの初期設定や処理を行う

*/
void BulletManager::start()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mBullets[i]->start();
	}
}

/*
@brief	弾の当たり判定を行う関数

@param[in]	Player *player_p:プレイヤークラスのポインタ
@param[in]	BossEnemy *bossEnemy_p:ボス敵クラスのポインタ
@param[in]	MinionEnemyManager *minionEnemyManager_p:雑魚敵管理クラスのポインタ
@note	生成した全てのバレットクラスの当たり判定処理を行う
@note	引数でクラスポインタから座標とサイズを参照している
@note	プレイヤー側と敵側で当たり判定関数に渡すの引数の変更している

*/
void BulletManager::checHit(Player *player_p, BossEnemy *bossEnemy_p,
                            MinionEnemyManager *minionEnemyManager_p)
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		if (!mBullets[i]
		         ->mIsActive) // 当たり判定を実行するクラスが有効じゃなければスルー
		{
			continue;
		}

		if (mBullets[i]->mIsPlayerBullet) // プレイヤー側の弾
		{
			mBullets[i]->hitCheck(bossEnemy_p); // ボス敵と
			checkHitMinionEnemy(minionEnemyManager_p,
			                    mBullets[i]); // 雑魚敵と
		}
		else // 敵側の弾
		{
			mBullets[i]->hitCheck(player_p); // プレイヤーと
		}
	}
}
/*
@brief	弾の発射に必要な位置や角度などの設定を行う関数

@param[in]	int setPositionX:設定するx座標
@param[in]	int setPositionY:設定するy座標
@param[in]	int setAngle:設定する角度
@param[in]	BULLET_TYPE bulletType:弾の種類
@param[in]	bool isBossEnemy:ボス敵が発射したかどうか
@param[in]	bool isPlayerBullet:プレイヤーが発射したかどうか

@note	フラグ（mIsActive）が true の場合は処理を行わず次の弾へ
@note
フラグ(mIsActive)をtrueにしてaction関数及び弾クラスの関数の処理を有効にしている
@note	引数で渡された位置・角度をもとに各パラメータを設定する
*/
void BulletManager::setBullet(int setPositionX, int setPositionY, int setAngle,
                              BULLET_TYPE bulletType, bool isBossEnemy,
                              bool isPlayerBullet)
{
	// フラグのたっていないものを探し、1つだけ処理を実行
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		if (mBullets[i]->mIsActive) // 有効化されているものはスルー
		{
			continue;
		}
		// 発射SEの再生
		PlaySoundMem(Data::getInstance()->mPlayerShotSoundEffectHandle,
		             DX_PLAYTYPE_BACK, TRUE);
		// 各パラメータの設定
		mBullets[i]->mX = setPositionX;                      // ｘ座標
		mBullets[i]->mY = setPositionY;                      // y座標
		mBullets[i]->mBulletType = bulletType;               // 弾の種類
		mBullets[i]->mWidth = mBulletWidthList[bulletType];  // 横幅
		mBullets[i]->mHeight = mBulletHightList[bulletType]; // 縦幅
		mBullets[i]->mSpeed = mBulletSpeedList[bulletType];  // 速度
		// ラジアンに変換
		double moveRadian = setAngle / DEGREE_TO_RADIAN_DIVISOR * PI;

		mBullets[i]->mAngle = setAngle; // 角度（ディグリー）
		mBullets[i]->mVectorX =
		    cos(moveRadian) * mBullets[i]->mSpeed; // ｘベクトル
		mBullets[i]->mVectorY =
		    sin(moveRadian) * mBullets[i]->mSpeed;     // ｙベクトル
		mBullets[i]->mIsActive = true;                 // 有効化判定
		mBullets[i]->mIsBossEnemy = isBossEnemy;       // ボス判定
		mBullets[i]->mIsPlayerBullet = isPlayerBullet; // プレイヤー側の弾か判定

		return; // 設置処理したからループを抜ける
	}
}
/*
@brief	雑魚敵との弾の当たり判定を行う関数

@param[in]	MinionEnemyManager *minionEnemyManager:雑魚敵管理クラスのポインタ
@param[in]	Bullet *bullet:弾クラスのポインタ
@note	有効化されている雑魚敵全てとの当たり判定処理を行う
@note	引数でクラスポインタから座標とサイズを参照している

*/
void BulletManager::checkHitMinionEnemy(MinionEnemyManager *minionEnemyManager,
                                        Bullet *bullet)
{
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)//インスタンスした数繰り返す
	{
		if (!minionEnemyManager->mEnemies[i]->mIsActive)//有効化されていなければスルー
		{
			continue;
		}
		bullet->hitCheck(minionEnemyManager->mEnemies[i]); // 通常敵と
	}
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)//インスタンスした数繰り返す
	{
		if (!minionEnemyManager->mTraceEnemies[i]->mIsActive)//有効化されていなければスルー
		{
			continue;
		}
		bullet->hitCheck(minionEnemyManager->mTraceEnemies[i]); // 追従敵と
	}
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)//インスタンスした数繰り返す
	{
		if (!minionEnemyManager->mChargeEnemies[i]->mIsActive)//有効化されていなければスルー
		{
			continue;
		}
		bullet->hitCheck(minionEnemyManager->mChargeEnemies[i]); // チャージ敵と
	}
}
