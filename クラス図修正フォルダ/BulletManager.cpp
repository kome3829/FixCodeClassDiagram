#include "BulletManager.h"





BulletManager::BulletManager()
{
    for (int i = 0; i < MAX_BULLET_NUMBER;i++)
    {
		mBullets[i] = new Bullet();
    }
}

BulletManager::~BulletManager()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		delete mBullets[i];
		mBullets[i] = nullptr;
	}
}

void BulletManager::action()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mBullets[i]->action();
	}
}

void BulletManager::draw()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mBullets[i]->draw();
	}
}

void BulletManager::start()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mBullets[i]->start();
	}
}

void BulletManager::checHit(Player *player_p, BossEnemy *bossEnemy_p,
                             MinionEnemyManager *minionEnemyManager_p)
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		if (!mBullets[i]->mIsActive)//有効じゃなければスルー
		{
			continue;
		}
		switch (mBullets[i]->mBulletType)//タイプによって引数の変更
		{
		case BULLET_TYPE::PLAYER_NOMAL:
			mBullets[i]->hitCheck(bossEnemy_p);//ボス敵と
			checkHitMinionEnemyPre(minionEnemyManager_p, mBullets[i]);//雑魚敵と
			break;
		case BULLET_TYPE::PLAYER_MISSILE:
			mBullets[i]->hitCheck(bossEnemy_p);
			checkHitMinionEnemyPre(minionEnemyManager_p,
			                       mBullets[i]); // 雑魚敵と
			break;
		case BULLET_TYPE::PLAYER_SPECIAL:
			mBullets[i]->hitCheck(bossEnemy_p);
			checkHitMinionEnemyPre(minionEnemyManager_p,
			                       mBullets[i]); // 雑魚敵と
			break;
		case BULLET_TYPE::ENEMY_NOMAL:
			mBullets[i]->hitCheck(player_p);
			break;
		case BULLET_TYPE::ENEMY_MINI_ORANGE:
			mBullets[i]->hitCheck(player_p);
			break;
		case BULLET_TYPE::ENEMY_MINI_YELLOW:
			mBullets[i]->hitCheck(player_p);

			break;

		default:
			break;
		}

	}
}

void BulletManager::setBullet(int setPositionX, int setPositionY, int setAngle,
                              BULLET_TYPE bulletType,bool isBossEnemy)
{
	// フラグのたっていないものを探し、1つだけsetbullet関数を実行
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		if (mBullets[i]->mIsActive)//有効化されているものはスルー
		{
			continue;
		}
		// 発射SEの再生
		PlaySoundMem(Data::getInstance()->mPlayerShotSoundEffectHandle,
		             DX_PLAYTYPE_BACK, TRUE);
		// 各パラメータの設定
		mBullets[i]->mX = setPositionX;//ｘ座標
		mBullets[i]->mY = setPositionY;	//y座標
		mBullets[i]->mBulletType = bulletType;//弾の種類
		mBullets[i]->mWidth = mBulletWidthList[bulletType];//横幅
		mBullets[i]->mHeight = mBulletHightList[bulletType];//縦幅
		mBullets[i]->mSpeed = mBulletSpeedList[bulletType];//速度
		// ラジアンに変換
		double moveRadian = setAngle / DEGREE_TO_RADIAN_DIVISOR * PI;

		mBullets[i]->mAngle = setAngle;//角度（ディグリー）
		mBullets[i]->mVectorX = cos(moveRadian) * mBullets[i]->mSpeed;//ｘベクトル
		mBullets[i]->mVectorY = sin(moveRadian) * mBullets[i]->mSpeed;//ｙベクトル
		mBullets[i]->mIsActive = true;//有効化判定
		mBullets[i]->mIsBossEnemy = isBossEnemy;//ボス判定
		return;//設置したからループを抜ける
	}
}

void BulletManager::checkHitMinionEnemyPre(MinionEnemyManager *minionEnemyManager_p,
                                      Bullet *mBullets)
{
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		if (!minionEnemyManager_p->mEnemies[i]->mIsActive)
		{
			continue;
		}
		mBullets->hitCheck(minionEnemyManager_p->mEnemies[i]); // 通常敵と
	}
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		if (!minionEnemyManager_p->mTraceEnemies[i]->mIsActive)
		{
			continue;
		}
		mBullets->hitCheck(minionEnemyManager_p->mTraceEnemies[i]); // 通常敵と
	}
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		if (!minionEnemyManager_p->mChargeEnemies[i]->mIsActive)
		{
			continue;
		}
		mBullets->hitCheck(minionEnemyManager_p->mChargeEnemies[i]); // 通常敵と
	}
}
