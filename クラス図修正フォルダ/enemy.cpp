#include "enemy.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note	各種初期設定。クラス生成の際に必要なパラメータはすべて設定する
@note	ゴミ値が入らないように変数の初期化を行う
*/
Enemy::Enemy()
{
	mArcMotionAngle = 0.0f;
	mArcMotionRadius = 0.0f;
	mAlpha = MAX_ALPHA;
	mX = 0;
	mY = 0;
	mWidth = ENEMY_WIDTH;
	mHeight = ENEMY_HEIGHT;
	mHitPoint = ENEMY_MAX_HP;
	mIsActive = false;
	mMoveCount = 0;
	mIsDefeat = false;
	mNumber = 0;
	mShotCount = 0;
	mIsTakeDamage = false;
	mDamageDisplayCount = 0;
	mVectorX = ENEMY_VECTOR_X;
	mVectorY = ENEMY_VECTOR_Y;
	mIsBossStage = false;
	mIsReachedStartPosition = false;
	mIsReachedBossPosition = false;
	mIsInvincible = false;
	mIsDamageCoolDown = false;
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note	クラス内メンバは自動で解放されるので特に記載なし
*/
Enemy::~Enemy()
{
}

/*
@brief	敵の移動および状態更新を行う（メインループ内で毎フレーム実行）

@param	なし
@return	なし

@note

  - フラグ（FLG）が true の場合のみ処理を実行する
  - MoveCount を用いて移動タイミングや動作を制御している
  - 画面外に出た場合は敵を消滅させる
  - ステージの種類に応じて移動処理を分岐する

    1. 通常ステージ
       - 一定フレームごとに移動し、出現位置の反対側へ向かって移動する

    2. ボスステージ
       - ボスに召喚されて指定位置まで移動する
       -
指定位置に移動後、一定カウント内までに倒されなかった場合、画面外へ移動する

*/
void Enemy::action(int *score)
{
	if (!mIsActive)
	{
		return;
	}
	takeDamage(score);//ダメージ処理
	mMoveCount++; // 移動カウントの更新
	// 通常ステージの処理
	if (!mIsBossStage)
	{
		if (mMoveCount % MOVE_INTERVAL_FRAME == 0) // ２フレームに一回移動
		{
			// y100まで降下
			if (mY <= ENEMY_DESCEND_LIMIT_Y)
			{
				mVectorY *= ENEMY_SLOWDOWN_Y; // 徐々に減速
				mY += mVectorY;
			}
			else
			{
				// 出現位置と反対側へ移動
				if (mNumber % EVEN_NUMBER == 0)
				{
					mX += mVectorX;
				}
				else
				{
					mX -= mVectorX;
				}
				// 一定の速度まで加速
				if (mVectorX <= ENEMY_SPEEDUP_LIMIT_X)
				{
					mVectorX *= ENEMY_SPEEDUP_X;
				}
			}
			// 画面外処理
			if (mX >= MAX_SCREEN_WIDTH + mWidth || mX <= 0 - mWidth ||
			    mY >= MAX_SCREEN_HEIGHT + mHeight || mY <= 0 - mHeight)
			{
				mIsActive = false;
				mIsDefeat = true;
			}
		}
	}
	// ボスステージの処理
	else
	{
		if (mNumber == SUMMON_LEFT) // 左側
		{
			// 位置についたか判定
			// 現在の位置と指定位置のずれが一定値になるまで処理を実行する
			if (abs((int)mX - ENEMY_SUMMON_POINT_LEFT_X) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    abs((int)mY - ENEMY_SUMMON_POINT_LEFT_Y) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    !mIsReachedBossPosition)
			{
				// atan2を使用して敵と指定位置とのなす角を計算
				double vectorTargetX = (ENEMY_SUMMON_POINT_LEFT_X - mX);
				double vectorTargetY = (ENEMY_SUMMON_POINT_LEFT_Y - mY);
				double targetAngle = atan2(vectorTargetY, vectorTargetX);
				// 角度から速度（Vx,Vy）を計算して設定している
				mVectorX = cos(targetAngle) * ENEMY_POSITION_MOVE_SPEED;
				mVectorY = sin(targetAngle) * ENEMY_POSITION_MOVE_SPEED;
				// 位置(X,Y)に速度(Vx,Vy)を加算して、移動している
				mX += mVectorX;
				mY += mVectorY;
				mShotCount = 0; // 発射カウントのリセット
			}
			else
			{
				// 位置に着いた場合移動行動フラグを切り替え
				mIsReachedBossPosition = true;
			}
			// 250フレームなると処理を実行
			// 画面外へ移動するよう速度を設定
			if (mMoveCount == ENEMY_EXIT_START_FRAME)
			{
				mVectorX = -(ENEMY_EXIT_SPD);
				mVectorY = -(ENEMY_EXIT_SPD);
			}
			// 250フレームを超えるとで画面外へ移動
			if (mMoveCount > ENEMY_EXIT_START_FRAME)
			{
				mShotCount = 0; // 発射カウントのリセット。攻撃の停止
				mX += mVectorX;
				mY += mVectorY;
				mVectorY *= ENEMY_SPEEDUP_Y; // 徐々に加速
			}
		}
		// マジックナンバー
		if (mNumber == SUMMON_RIGHT) // 右側
		{
			// 位置についたか判定
			// 現在の位置と指定位置のずれが一定値になるまで処理を実行する
			if (abs((int)mX - ENEMY_SUMMON_POINT_RIGHT_X) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    abs((int)mY - ENEMY_SUMMON_POINT_RIGHT_Y) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    !mIsReachedBossPosition)
			{
				// atan2を使用して敵と指定位置とのなす角を計算
				double vectorTargetX = (ENEMY_SUMMON_POINT_RIGHT_X - mX);
				double vectorTargetY = (ENEMY_SUMMON_POINT_RIGHT_Y - mY);
				double targetAngle = atan2(vectorTargetY, vectorTargetX);
				// 角度から速度（Vx,Vy）を計算して設定している
				mVectorX = cos(targetAngle) * ENEMY_POSITION_MOVE_SPEED;
				mVectorY = sin(targetAngle) * ENEMY_POSITION_MOVE_SPEED;
				// 位置(X,Y)に速度(Vx,Vy)を加算して、移動している
				mX += mVectorX;
				mY += mVectorY;
				mShotCount = 0; // 発射カウントのリセット
			}
			else
			{
				// 位置に着いた場合移動行動フラグを切り替え
				mIsReachedBossPosition = true;
			}
			// 250フレームなると処理を実行
			// 画面外へ移動するよう速度を設定
			if (mMoveCount == ENEMY_EXIT_START_FRAME)
			{
				mVectorX = ENEMY_EXIT_SPD;
				mVectorY = -(ENEMY_EXIT_SPD);
			}
			// 250フレームを超えるとで画面外へ移動
			if (mMoveCount > ENEMY_EXIT_START_FRAME)
			{
				mShotCount = 0; // 発射カウントのリセット。攻撃の停止
				mX += mVectorX;
				mY += mVectorY;
				mVectorY *= ENEMY_SPEEDUP_Y; // 徐々に加速
			}
		}
	}
	// オーバフロー防止
	if (mMoveCount == MOVE_COUNT_LIMIT)
	{
		mMoveCount = 0; // 移動カウントのリセット
	}
}

/*
@brief	ゲームシーンの描画ループ内で実行される、敵の描画処理を行う関数

@param	なし

@return		なし

@note
  - ゲームシーンの描画ループ内で実行される、敵の描画処理を行う関数
  - フラグ（FLG）が true の場合のみ処理を実行する
  - ダメージフラグ（dmgFlg）が true
の場合は、一定時間ダメージ用の敵画像を表示する
  - なにもなければは通常の敵画像を表示する

*/
void Enemy::draw()
{
	if (mIsActive)
	{
		// ダメージ時の場合
		if (mIsDamageCoolDown)
		{
			mDamageDisplayCount++; // ダメージ表示カウントの更新
			// ダメージ画像の表示
			DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
			          Data::getInstance()->mEnemyDamageImageHandle, TRUE);
			// ダメージ画像表示フレームを制限
			if (mDamageDisplayCount > DAMAGE_COUNT_LIMIT)
			{
				// ダメージフラグ、カウントのリセット
				mIsDamageCoolDown = false;
				mDamageDisplayCount = 0;
			}
		}
		else
		{
			// 通常画像の表示
			DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
			          Data::getInstance()->mEnemyImageHandle, TRUE);
		}
	}
}

/*
@brief	動作開始時に位置・体力・フラグなど各パラーメータの初期設定を行う関数

@param		なし
@return		なし

@note     敵の生成時や、状態を初期状態に戻す際に使用される。
*/
void Enemy::start()
{
	mX = 0;
	mY = 0;
	mWidth = ENEMY_WIDTH;
	mHeight = ENEMY_HEIGHT;
	mHitPoint = ENEMY_MAX_HP;
	mIsActive = false;
	mMoveCount = 0;
	mIsDefeat = false;
	mNumber = 0;
	mShotCount = 0;
	mIsTakeDamage = false;
	mDamageDisplayCount = 0;
	mVectorX = ENEMY_VECTOR_X;
	mVectorY = ENEMY_VECTOR_Y;
	mIsReachedStartPosition = false;
	mIsBossStage = false;
	mIsReachedBossPosition = false;
	mIsInvincible = false;
	mIsDamageCoolDown = false;
}

/*
@brief	敵を出現させるための初期設定を行う関数

@param[in]	int 	popPositionX			:出現させるX座標
@param[in]	int 	popPositionY			:出現させるy座標
@param[in]	int 	number		:敵個体の判別番号
@param[in]	bool 	isBossStage	:ボスステージ判別フラグ

@return		なし

@note     フラグ（FLG）を true にすることで、action 関数および draw
関数の処理対象となる
@note     引数で位置、敵の番号、ボスステージかどうかを指定できる
@note
ボスステージフラグ（BOSS_STAGE）に応じて、位置・番号・体力などのパラメータ設定処理を切り替える
@warning
- 本関数は、継承先の敵クラスでも共通の出現処理として使用される
*/
void Enemy::pop(int popPositionX, int popPositionY, int number,
                bool isBossStage)
{
	if (!isBossStage)
	{
		// パラメータの設定
		mX = popPositionX;
		mY = popPositionY;
		mIsActive = true;
		mNumber = number;
		mIsDefeat = false;
	}
	// ボスステージの場合
	else
	{
		// パラメータの設定
		mIsBossStage = isBossStage;
		mX = popPositionX;
		mY = popPositionY;
		mIsActive = true;
		mNumber = number;
		mIsDefeat = false;
		mShotCount = BOSS_STAGE_START_COUNT;
		mMoveCount = BOSS_STAGE_START_COUNT;
		mIsTakeDamage = false;
		mIsReachedStartPosition = true;
		mHitPoint = ENEMY_MAX_HP_BOSS_STAGE;
	}
}

/*
@brief	敵とプレイヤー弾との当たり判定を行う関数

@param[in]	Bullet* 		bullet			:通常弾クラスのインスタンスポインタ
@param[in]	MissileBullet* 	missileBullet
:ミサイル弾クラスのインスタンスポインタ
@param[in]	SpecialBullet* 	specialBullet
:スペシャル弾クラスのインスタンスポインタ
@param[in]	HomingBullet* 	homingBullet
:ホーミング弾クラスのインスタンスポインタ
@param[in]	int* 			score	:ゲームスコアのポインタ

@return		ヒット判定:bool :当たった　true/当たってない　false

@note

  - フラグ（FLG）および各プレイヤー弾のフラグ（flg）が true
の場合のみ処理を実行する
  - プレイヤー弾の種類に応じて処理を分岐し、それぞれ当たり判定を行う
  - 敵と弾の位置の差をもとに一定範囲内に入った場合、ヒットと判定する
  - ヒット判定に応じて、ダメージ処理と撃破判定を行う
@warning
- 本関数は、継承先の敵クラスでも共通の出現処理として使用される
- ヒットの有無を外部で判定できるように、返り値は bool 型としている
*/
bool Enemy::checkPlayerBulletHit(Bullet *bullet, MissileBullet *missileBullet,
                                 SpecialBullet *specialBullet,
                                 HomingBullet *homingBullet, int *score)
{
	// 通常弾の判定

	if (checkHitPbltPre(bullet))
	{
		takeDamagePre(score);
		return true;
	}

	// ミサイル弾の判定
	// 敵とミサイル弾の位置を計算して弾が一定範囲内なら当たり判定を返す
	if (checkHitPbltPre(missileBullet))
	{
		takeDamagePre(score);
		return true;
	}
	// スペシャル弾の判定
	if (checkHitPbltPre(specialBullet))
	{
		takeDamagePre(score);
		return true;
	}

	// ホーミング弾の判定
	if (checkHitPbltPre(homingBullet))
	{
		takeDamagePre(score);
		return true;
	}

	return false;
}

/*
@brief	撃破SEを再生する関数。hitPlayerBullet関数で使用

@param	なし

@return		なし

*/
void Enemy::playSoundEffect()
{
	PlaySoundMem(Data::getInstance()->mDefeatEnemySoundEffectHandle,
	             DX_PLAYTYPE_BACK, TRUE);
}

bool Enemy::checkHitPbltPre(Bullet *bullet)
{
	// 通常弾の判定
	if (!mIsActive)
	{
		return false;
	}
	return bullet->hitCheckPre(mX, mY, mWidth, mHeight);
}

void Enemy::takeDamagePre(int *score)
{
	// 無敵フラグ(Unbeatable)がfalseの場合はダメージ処理・スコア加算・を行う
	if (mIsInvincible)
	{
		return;
	}
	*score += (int)HIT_SCORE;
	if (!mIsTakeDamage)
	{
		mIsTakeDamage = true;
		mIsDamageCoolDown = true;
	}

	mHitPoint -= ENEMY_TAKE_DAMAGE;
	// HPが０以下の場合は撃破処理（スコア加算・SE再生・フラグ更新）を行う
	if (mHitPoint <= 0)
	{
		mIsDefeat = true;
		*score += (int)ENEMY_SCORE;
		playSoundEffect();
		mIsActive = false;
	}
}

void Enemy::takeDamage(int *score)
{
	if (!mIsTakeDamage)
	{
		return;
	}
	// 無敵フラグ(Unbeatable)がfalseの場合はダメージ処理・スコア加算・を行う
	if (!mIsInvincible && !mIsDamageCoolDown)
	{
		*score += (int)HIT_SCORE;
		mIsDamageCoolDown = true;
		mHitPoint -= ENEMY_TAKE_DAMAGE;
	}
		mIsTakeDamage = false;
	// HPが０以下の場合は撃破処理（スコア加算・SE再生・フラグ更新）を行う
	if (mHitPoint <= 0)
	{
		mIsDefeat = true;
		*score += (int)ENEMY_SCORE;
		playSoundEffect();
		mIsActive = false;
	}
}
