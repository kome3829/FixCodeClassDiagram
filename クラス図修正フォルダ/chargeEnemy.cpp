#include "chargeEnemy.h"
/*
@brief	コンストラクタ

@param	    なし
@return		なし

@note	各種初期設定。クラス生成の際に必要なパラメータはすべて設定する
@note	ゴミ値が入らないように変数の初期化を行う
*/
ChargeEnemy::ChargeEnemy()
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
	mVectorX = CHARGE_ENEMY_VECTOR_X;
	mVectorY = CHARGE_ENEMY_VECTOR_Y;
	mIsBossStage = false;
	mIsReachedBossPosition = false;
	mIsInvincible = false;
	mAlpha = MAX_ALPHA;
	mArcMotionAngle = 0.0f;
	mIsDamageCoolDown = false;
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note	クラス内メンバは自動で解放されるので特に記載なし
*/
ChargeEnemy::~ChargeEnemy()
{
}

/*
@brief	メインループで実行する更新処理を行う関数

@param     	なし
@return     なし
@note		フラグ（flg）がtrueの場合のみ処理を実行する
@note		画面外でのフラグを管理
@note		ステージに応じて、移動処理を分岐している

@note
1. 通常ステージ
    - 左右2か所の出現位置から反対側の画面外へ放物線を描きながら移動

2. ボスステージ　
    - ボスに左右に召喚され、指定のポジションまで移動
    -
攻撃までに倒されなければワープして画面端から攻撃しながら反対側の画面端へ弧を描きながら移動

@warning
- 他雑魚敵と違いボスステージでの移動パターンはボス攻撃と合わせた動きにしている

*/
void ChargeEnemy::action(int *score)
{
	if (!mIsActive)
	{
		return;
	}
	takeDamage(score); // ダメージ処理
	mMoveCount++;      // 移動カウントの更新
	                   // 通常ステージの処理
	if (!mIsBossStage)
	{
		if (mMoveCount % MOVE_INTERVAL_FRAME == 0) // ２フレームに一回移動
		{
			// 左右の判別
			if (mNumber % EVEN_NUMBER == 0)
			{
				// 画面中央で速度(Vy)の加速度を切り替えて放物線を描くようしている
				if (mX <= SCREEN_CENTER_X)
				{
					mVectorY *= ENEMY_SLOWDOWN_Y; // 徐々に減速
					mY += mVectorY;
					mX += mVectorX;
				}
				else
				{
					mVectorY *= ENEMY_SPEEDUP_Y; // 徐々に加速
					mY -= mVectorY;
					mX += mVectorX;
				}
			}
			else
			{
				// 画面中央で速度(Vy)の加速度を切り替えて放物線を描くようしている
				if (mX >= SCREEN_CENTER_X)
				{
					mVectorY *= (ENEMY_SLOWDOWN_Y +
					             CHARGE_ENEMY_SPD_MARGIN); // 徐々に減速
					mY += mVectorY;
					mX -= mVectorX;
				}
				else
				{
					mVectorY *= (ENEMY_SPEEDUP_Y -
					             CHARGE_ENEMY_SPD_MARGIN); // 徐々に加速
					mY -= mVectorY;
					mX -= mVectorX;
				}
			}
			// 画面外の判定
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
			if (abs((int)mX - CHARGE_ENEMY_SUMMON_POINT_LEFT_X) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    abs((int)mY - CHARGE_ENEMY_SUMMON_POINT_LEFT_Y) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    !mIsReachedBossPosition)
			{
				// atan2を使用して敵と指定位置とのなす角を計算
				double vectorTargetX = (CHARGE_ENEMY_SUMMON_POINT_LEFT_X - mX);
				double vectorTargetY = (CHARGE_ENEMY_SUMMON_POINT_LEFT_Y - mY);
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
			// 331フレームなると処理を実行
			if (mMoveCount > ARC_MOVE_START_FRAME)
			{
				// 円運動移動
				mArcMotionAngle += ANGLE_SPEED_LEFT;
				mX = LEFT_CIRCLE_CENTER_POSITION_X +
				     cos(mArcMotionAngle) * mArcMotionRadius;
				mY = LEFT_CIRCLE_CENTER_POSITION_Y +
				     sin(mArcMotionAngle) * mArcMotionRadius;
			}
		}
		if (mNumber == SUMMON_RIGHT) // 右側
		{
			// 位置についたか判定
			// 現在の位置と指定位置のずれが一定値になるまで処理を実行する
			if (abs((int)mX - CHARGE_ENEMY_SUMMON_POINT_RIGHT_X) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    abs((int)mY - CHARGE_ENEMY_SUMMON_POINT_RIGHT_Y) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    !mIsReachedBossPosition)
			{
				// atan2を使用して敵と指定位置とのなす角を計算
				double vectorTargetX = (CHARGE_ENEMY_SUMMON_POINT_RIGHT_X - mX);
				double vectorTargetY = (CHARGE_ENEMY_SUMMON_POINT_RIGHT_Y - mY);
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
			// 331フレームなると処理を実行
			if (mMoveCount > ARC_MOVE_START_FRAME)
			{
				// 円運動移動
				mArcMotionAngle += ANGLE_SPEED_RIGHT;
				mX = RIGHT_CIRCLE_CENTER_POSITION_X +
				     cos(mArcMotionAngle) * mArcMotionRadius;
				mY = RIGHT_CIRCLE_CENTER_POSITION_Y +
				     sin(mArcMotionAngle) * mArcMotionRadius;
			}
		}
		// 画面外判定
		if (mX >= MAX_SCREEN_WIDTH + mWidth || mX <= 0 - mWidth ||
		    mY >= MAX_SCREEN_HEIGHT + mHeight || mY <= 0 - mHeight)
		{
			mIsActive = false;
			mIsDefeat = true;
		}
		// チャージ後の攻撃終了時まで無敵
		if (mMoveCount > UNBEATABLE_START_FRAME &&
		    mMoveCount <= UNBEATABLE_END_FRAME)
		{
			mIsInvincible = true;
		}
		// ワープ移動(280～330フレームの間)
		else if (mMoveCount > WARP_START_FRAME && mMoveCount <= WARP_END_FRAME)
		{
			// ダメージ用画像を表示するため、ダメージフラグをtrue
			mIsTakeDamage = true;

			// ワープ演出の開始(304フレーム時)
			if (mMoveCount == WARP_TRIGGER_FRAME)
			{
				mVectorY = BOSS_ENEMY_WARP_VECTOR_Y; // y軸の速度を設定
			}
			// ワープ演出
			if (mMoveCount > WARP_TRIGGER_FRAME && mAlpha > 0)
			{
				// 徐々に薄くなる＆徐々に上昇
				mAlpha -= FADE_ALPHA_STEP;
				mY += mVectorY;
				mVectorY *= BOSS_ENEMY_SPEEDUP_Y;
			}
		}
		// 円運動開始フレーム(331フレーム)
		if (mMoveCount == ARC_MOVE_START_FRAME)
		{
			if (mNumber == SUMMON_LEFT) // 左側
			{
				// ワープ移動
				mX = CHARGE_ENEMY_WARP_POINT_LEFT_X;
				mY = CHARGE_ENEMY_WARP_POINT_LEFT_Y;
				// 円運動計算に必要な半径、角度を計算。
				double vectorToCenterX = CHARGE_ENEMY_WARP_POINT_LEFT_X -
				                         LEFT_CIRCLE_CENTER_POSITION_X;
				double vectorToCenterY = CHARGE_ENEMY_WARP_POINT_LEFT_Y -
				                         LEFT_CIRCLE_CENTER_POSITION_Y;

				mArcMotionRadius = sqrt(vectorToCenterX * vectorToCenterX +
				                        vectorToCenterY * vectorToCenterY);
				mArcMotionAngle = atan2(vectorToCenterY, vectorToCenterX);
			}
			else if (mNumber == SUMMON_RIGHT) // 右側
			{
				// ワープ移動
				mX = CHARGE_ENEMY_WARP_POINT_RIGHT_X;
				mY = CHARGE_ENEMY_WARP_POINT_RIGHT_Y;
				// 円運動計算に必要な半径、角度を計算。
				double vectorToCenterX = CHARGE_ENEMY_WARP_POINT_RIGHT_X -
				                         RIGHT_CIRCLE_CENTER_POSITION_X;
				double vectorToCenterY = CHARGE_ENEMY_WARP_POINT_RIGHT_Y -
				                         RIGHT_CIRCLE_CENTER_POSITION_Y;

				mArcMotionRadius = sqrt(vectorToCenterX * vectorToCenterX +
				                        vectorToCenterY * vectorToCenterY);
				mArcMotionAngle = atan2(vectorToCenterY, vectorToCenterX);
			}
		}
	}
	// オーバーフロー防止
	if (mMoveCount == MOVE_COUNT_LIMIT)
	{
		mMoveCount = 0;
	}
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param[in]	なし
@return		なし

@note	フラグ(flg)がtrueの場合のみ処理を実行する
@note	ダメージフラグ(dmgFlg)がtrueの場合はダメージ用の画像を表示する
@note	なにもなければ、通常のチャージ攻撃敵の画像を表示する
*/
void ChargeEnemy::draw()
{
	if (mIsActive)
	{
		if (mIsDamageCoolDown)
		{
			mDamageDisplayCount++; // ダメージ表示カウントの更新

			// ワープ移動の演出のため、アルファ値の設定
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, mAlpha);
			// ダメージ画像の表示
			DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
			          Data::getInstance()->mEnemyDamageImageHandle, TRUE);
			// アルファ値のリセット
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
			          Data::getInstance()->mChargeEnemyImageHandle, TRUE);
		}
	}
}

/*
@brief	処理開始に必要なパラーメータの初期設定や処理を行う関数

@param[in]	なし
@return		なし
*/
void ChargeEnemy::start()
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
	mVectorX = CHARGE_ENEMY_VECTOR_X;
	mVectorY = CHARGE_ENEMY_VECTOR_Y;
	mIsBossStage = false;
	mIsReachedBossPosition = false;
	mIsInvincible = false;
	mAlpha = MAX_ALPHA;
	mArcMotionAngle = 0.0f;
	mIsDamageCoolDown = false;
}
