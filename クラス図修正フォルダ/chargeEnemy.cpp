#include "ChargeEnemy.h"
#include "BulletManager.h"
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
	mIsWarp = false;
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
@note		フラグ（mIsActive）がtrueの場合のみ処理を実行する
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
void ChargeEnemy::action(EnemyActionData *actiondata)
{
	if (!mIsActive)
	{
		return;
	}
	takeDamage(actiondata->mScore, actiondata->mEffectManager, actiondata->mItemObjectManager,
	           actiondata->mPlayer); // ダメージ処理
	shotChargeEnemyBullet(actiondata->mBulletManager, actiondata->mPlayer,
	                      actiondata->mEffectManager,
	                      actiondata->mItemObjectManager); // 弾発射処理
	mMoveCount++;                             // 移動カウントの更新
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
			// ダメージ用画像を表示するため、ワープフラグをtrue
			mIsWarp = true;

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
			mIsWarp = false;            // ワープ最中フラグをfalseに
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

@note	フラグ(mIsActive)がtrueの場合のみ処理を実行する
@note
ダメージフラグ(mIsDamageCoolDown)がtrueの場合はダメージ用の画像を表示する
@note	なにもなければ、通常のチャージ攻撃敵の画像を表示する
*/
void ChargeEnemy::draw()
{
	if (!mIsActive)
	{
		return;
	}
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
	else if (mIsWarp)
	{
		// ワープ移動の演出のため、アルファ値の設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, mAlpha);
		// ダメージ画像の表示
		DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
		          Data::getInstance()->mEnemyDamageImageHandle, TRUE);
		// アルファ値のリセット
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	else
	{
		// 通常画像の表示
		DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
		          Data::getInstance()->mChargeEnemyImageHandle, TRUE);
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
	mIsWarp = false;
}

/*
@brief	チャージ攻撃敵の弾発射を管理する関数

@param[in]	BulletManager *bulletManager:弾管理クラスのポインタ
@param[in]	Player *player:プレイヤークラスのポインタ
@param[in]	EffectManager *effectManager:エフェクト管理クラスのポインタ
@return		なし

@note

- 弾を発射する敵のフラグ(mIsActive)がtrueの場合のみ処理を実行する
- mShotCount変数でカウントし、処理を管理している
- 発射パターンをステージ応じて切り替えている

1. 通常のステージ
    - 一定間隔でミニ弾の発射を繰り返している
    - プレイヤーを狙うようにミニ弾を連続で発射している
    - 発射するまでの間チャージエフェクト表示を行う

2. ボスステージ
    - ミニ弾の扇状の弾幕をプレイヤーを狙うように発射している
        - 一定間隔まで発射しないチャージ時間を設けている
        - 同時に３方向ミニ弾を発射している
        - 発射を開始すると１８回連続でミニ弾の弾幕を発射する
    - 通常弾の円状の弾幕を発射している
        - 一定間隔まで発射しないチャージ時間を設けている
        - 同時に10方向に通常弾を発射している
        - チャージが終わると間隔を空けて３回の発射を行う
    - 発射するまで間のチャージエフェクトとワープエフェクトの表示を行う
    -無敵アイテムを発射開始時にドロップしている

- 弾発射と同時に発射エフェクトの表示と発射SEの再生も行う
@warning
    - ボスステージではボスのチャージ攻撃と合わせて激しい攻撃を行う
    - 救済として無敵アイテムをドロップしている


*/
void ChargeEnemy::shotChargeEnemyBullet(BulletManager *bulletManager,
                                        Player *player,
                                        EffectManager *effectManager,
                                        ItemObjectManager *itemObjectManager)
{
	if (!mIsActive)
	{
		return;
	}
	// 通常ステージの処理
	if (!mIsBossStage)
	{
		mShotCount++; // 発射カウントの更新
		// 40～120フレームは溜め演出
		// 40フレームごとに処理を実行
		if (mShotCount % CHARGE_EF_INTERVAL == 0 && mShotCount <= CHARGE_EF_END)
		{
			effectManager->setEffect(&(mX), &(mY), CHARGE_EF);
		}

		// 120フレーム後なら発射処理
		if (mShotCount >= CHARGE_ENEMY_SHOT_START_FRAME)
		{
			// 10フレームの間隔を空けて発射処理
			if (mShotCount % CHARGE_ENEMY_SHOT_INTERVAL_FRAME == 0)
			{
				// プレイヤー狙いの角度を計算
				double vectorTargetX = (player->mX - mX);
				double vectorTargetY = (player->mY - mY);
				double targetAngle = atan2(vectorTargetY, vectorTargetX);
				int targetAngle_Degree =
				    (int)(targetAngle * DEGREE_TO_RADIAN_DIVISOR / PI);

				bulletManager->setBullet((int)mX, (int)mY, targetAngle_Degree,
				                         BULLET_TYPE::ENEMY_MINI_ORANGE, false,
				                         false);

				effectManager->setEffect(&(mX), &(mY), SHOT_EF);

				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}
		}
	}

	// ボスステージの処理
	else
	{
		// 移動カウントが240フレームまでチャージエフェクトの表示
		if (mMoveCount <= CHARGE_ENEMY_BOSS_CHARGE_END)
		{
			// エフェクト再生終了間隔と同じ感覚でエフェクトの表示を行う(40フレームごと)
			if (mMoveCount % CHARGE_EF_INTERVAL == 0 &&
			    mMoveCount >= CHARGE_EF_INTERVAL)
			{
				effectManager->setEffect(&(mX), &(mY), CHARGE_EF);
			}
		}

		// 移動カウントが304フレームでワープのエフェクト表示
		if (mMoveCount == CHARGE_ENEMY_BOSS_WARP_EF_FRAME)
		{
			effectManager->setEffect(&(mX), &(mY), WARP_EF);
		}
		// ワープエフェクトの表示終了フレーム(330)から攻撃処理開始
		if (mMoveCount > CHARGE_ENEMY_BOSS_ATTACK_START)
		{
			mShotCount++; // 発射カウント更新
			// 攻撃開始から１４フレーム後救済措置無敵アイテムをドロップ左右に一個ずつ
			if (mMoveCount == CHARGE_ENEMY_BOSS_ITEM_DROP_FRAME)
			{
				if (mNumber == SUMMON_LEFT)
				{
					itemObjectManager->setItemObject(
					    ITEM_DROP_LEFT_X, ITEM_DROP_Y, 0, OBJECT_STAR, player);
				}
				else if (mNumber == SUMMON_RIGHT)
				{
					itemObjectManager->setItemObject(
					    ITEM_DROP_RIGHT_X, ITEM_DROP_Y, 0, OBJECT_STAR, player);
				}
			}
			// 50フレーム間隔で実行
			if (mShotCount % CHARGE_ENEMY_RING_SHOT_INTERVAL == 0)
			{
				int ringShotAngle;
				// 発射を10発分繰り返す。同時発射
				// 円形の弾幕の発射
				for (int i = 0; i < CHARGE_ENEMY_RING_BULLET_AMOUNT; i++)
				{
					// 36度毎に発射角度を変更するよう設定
					ringShotAngle = i * CHARGE_ENEMY_RING_SHOT_ANGLE;

					bulletManager->setBullet((int)mX, (int)mY, ringShotAngle,
					                         ENEMY_NOMAL, true, false);
					effectManager->setEffect(&(mX), &(mY), SHOT_EF);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}
			// 発射カウントが80を超えたら実行
			// プレイヤー狙いのミニ弾の発射
			else if (mShotCount >= CHARGE_ENEMY_AIM_SHOT_START)
			{
				// 5フレーム間隔でミニ弾を発射
				if (mShotCount % CHARGE_ENEMY_AIM_SHOT_INTERVAL == 0)
				{
					// 発射を3回繰り返す。同時に発射
					// 扇状の弾幕を発射
					for (int i = 0; i < CHARGE_ENEMY_AIM_BULLET_AMOUNT; i++)
					{
						// プレイヤー狙いの角度を計算
						double vectorTargetX = (player->mX - mX);
						double vectorTargetY = (player->mY - mY);
						double targetAngle =
						    atan2(vectorTargetY, vectorTargetX);

						int targetAngle_Degree =
						    (int)(targetAngle * DEGREE_TO_RADIAN_DIVISOR / PI);

						bulletManager->setBullet(
						    (int)mX, (int)mY,
						    targetAngle_Degree + CHARGE_ENEMY_ANGLE_STEP * i -
						        CHARGE_ENEMY_ANGLE_STEP,
						    ENEMY_MINI_ORANGE, false, false);
						effectManager->setEffect(&(mX), &(mY), SHOT_EF);
					}
					// 発射SEの再生
					PlaySoundMem(
					    Data::getInstance()->mEnemyShotSoundEffectHandle,
					    DX_PLAYTYPE_BACK, TRUE);
				}
			}
		}
	}
	// カウントのリセット、170フレームで1巡
	if (mShotCount >= CHARGE_ENEMY_SHOT_RESET_FRAME)
	{
		mShotCount = 0; // カウントをリセット
	}
}
