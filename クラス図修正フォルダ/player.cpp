#include "Player.h"
#include "BulletManager.h"
/*
@brief	コンストラクタ

@param	　　なし
@return		なし

@note      各種初期設定。クラス生成の際に必要なパラメータはすべて設定する

*/
Player::Player()
{
	mX = SCREEN_CENTER_X;
	mY = SCREEN_CENTER_Y;
	mWidth = PLAYER_HIT_RANGE;
	mHeight = PLAYER_HIT_RANGE;
	mIsActive = true;
	mImageWidth = PLAYER_WIDTH;
	mImageHeight = PLAYER_HEIGHT;
	mSpeed = PLAYER_SPEED;
	mLife = MAX_LIFE;
	mIsActiveMissileShot = false;
	mIsTakeDamage = false;
	mIsDefeat = false;
	mDamageDisplayCount = 0;
	mShotPower = 0;
	mIsUnbeatable = false;
	mUnbeatableCount = 0;
	mUnbeatableGagePercent = PLAYER_UNBEATABLE_MAX_PERCENT;
	for (int i = 0; i < MAX_HP; i++)
	{
		mIsActiveLifeIcon[i] = TRUE;
	}
	mIsDamegeCoolDown = false;
	mIsItemHit = false;

	mNomalShotIntervalCount = 0;
	mNomalShotInterval = PLAYER_NORMAL_SHOT_SPEED;
	mMissileShotIntervalCount = 0;
	mSpecialShotCount = 0;
	mShotCount = 0;
	mHitItemType = 0;
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note      クラス内メンバは自動で解放されるので特に記載なし。
*/
Player::~Player()
{
}

/*
@brief	ゲームシーンのメインループで実行する更新処理を行うの関数

@param	なし
@return		なし

@note

- キーボード入力によってプレイヤーの移動処理を行う
    -
左右上下キーの内、押されたキーに応じて、位置（mX,mY）を移動速度(mSpeed)で加算、減算している
    -
シフトキーを同時に押すことで、移動速度(mSpeed)を遅くする事できるようにしている

- 無敵時間の更新処理を行う
    -
無敵時間のカウント(mUnbeatableGagePercent)と無敵フラグ（mIsUnbeatable）でタイミングや動作を制御している
    - 無敵フラグに応じて、無敵時間表示ゲージ用の変数の計算処理を行う
-ダメージ時の更新処理を行う
@warning
- 移動処理の分岐はせずに同時押し可能にすることで斜め移動も可能にしている
- 遅い移動で細かい移動操作を行えるようにしている
*/
void Player::action(BulletManager *bulletManager, EffectManager *effectManager,
                    int *score)
{
	if (mIsDefeat)
	{
		return;
	}
	takeDamage();                                   // ダメージ処理
	shotPlayerBullet(bulletManager, effectManager); // 弾発射処理
	applyItemEffect(score, effectManager);          // アイテム効果反映処理
	// player操作
	// 左移動
	if (checkPushkey(KEY_INPUT_LEFT))
	{
		mX -= mSpeed;
		// 画面外へ出ないように制限
		if (mX <= mImageWidth / CUT_HALF)
		{
			mX = mImageWidth / CUT_HALF;
		}
	}

	// 右移動
	if (checkPushkey(KEY_INPUT_RIGHT))
	{
		mX += mSpeed;
		// 画面外へ出ないように制限
		if (mX >= MAX_SCREEN_WIDTH - mImageWidth / CUT_HALF)
		{
			mX = MAX_SCREEN_WIDTH - mImageWidth / CUT_HALF;
		}
	}

	// 上移動
	if (checkPushkey(KEY_INPUT_UP))
	{
		mY -= mSpeed;
		// 画面外へ出ないように制限
		// 修正案。条件式に/2の追加。
		if (mY <= mImageHeight)
		{
			mY = mImageHeight;
		}
	}

	// 下移動
	if (checkPushkey(KEY_INPUT_DOWN))
	{
		mY += mSpeed;
		// 画面外へ出ないように制限
		if (mY >= MAX_SCREEN_HEIGHT - mImageHeight)
		{
			mY = MAX_SCREEN_HEIGHT - mImageHeight;
		}
	}

	// 左シフトで速度ダウン
	if (checkPushkey(KEY_INPUT_LSHIFT))
	{
		mSpeed = PLAYER_LOW_SPEED;
	}
	else
	{
		mSpeed = PLAYER_SPEED;
	}
	// ダメージ無効フラグがtrueの場合
	if (mIsDamegeCoolDown)
	{
		mDamageDisplayCount++; // ダメージカウントの更新
		if (mDamageDisplayCount ==
		    PLAYER_DAMAGE_RESET_FRAME) // ダメージカウントが40フレームでリセット
		{
			mIsDamegeCoolDown = false;
			mDamageDisplayCount = 0;
		}
	}

	// 無敵かつ無敵時間内なら
	if (mIsUnbeatable && mUnbeatableCount <= PLAYER_UNBEATABLE_TIME)
	{
		mUnbeatableCount++; // 無敵時間カウントの更新
	}
	// 無敵じゃない時間外、どちらかを満たしていれば
	else
	{
		// 無敵フラグ、無敵時間カウントのリセット
		mIsUnbeatable = false;
		mUnbeatableCount = 0;
	}
	// 無敵フラグがtrueの場合
	if (mIsUnbeatable)
	{
		// 無敵時間ゲージの表示割合の更新
		mUnbeatableGagePercent =
		    (PLAYER_UNBEATABLE_MAX_FRAME - (double)mUnbeatableCount) /
		    PLAYER_UNBEATABLE_MAX_FRAME;
	}
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note

- ダメージフラグ（mIsDamegeCoolDown）がtrueの場合はダメージ用の画像を表示する
- 無敵フラグ(mIsUnbeatable)がtrueの場合は無敵用の画像を表示する
    - 残り無敵時間のタイマーゲージを表示する
- なにもなければ通常の画像を表示する
- プレイヤーの残り残機を表示する

*/
void Player::draw()
{
	// playerの表示
	if (mIsDamegeCoolDown) // ダメージ用画像の表示
	{
		DrawGraph((int)mX - mImageWidth / CUT_HALF,
		          (int)mY - mImageHeight / CUT_HALF,
		          Data::getInstance()->mPleyerDamageImageHandle, true);
	}
	else if (mIsUnbeatable) // 無敵用画像の表示
	{
		// 残り時間が一定以下(60フレーム)になると、無敵時の画像と通常時の画像を交互に表示
		if (mUnbeatableCount < UNBEATABLE_FLASH_START_FRAME ||
		    (mUnbeatableCount / PLAYER_UNBEATABLE_FLASH_INTERVAL_FRAME) %
		            EVEN_NUMBER ==
		        0)
		{
			// 無敵用画像の表示
			DrawGraph((int)mX - mImageWidth / CUT_HALF,
			          (int)mY - mImageHeight / CUT_HALF,
			          Data::getInstance()->mPleyerUnbeatableImageHandle, true);
		}
		else
		{
			// 通常画像の表示
			DrawGraph((int)mX - mImageWidth / CUT_HALF,
			          (int)mY - mImageHeight / CUT_HALF,
			          Data::getInstance()->mPleyerImageHandle, true);
		}
		// 無敵時間表示ゲージ
		// プレイヤーの画像の下に表示のための位置の計算
		int unbeatableGageLeftX =
		    (int)mX - PLAYER_UNBEATABLE_GAUGE_LEFT_OFFSET; // 左端
		int unbeatableGageTopY =
		    (int)mY + PLAYER_UNBEATABLE_GAUGE_TOP_OFFSET; // 上端
		int unbeatableGageRightX = (int)mX -
		                           PLAYER_UNBEATABLE_GAUGE_LEFT_OFFSET +
		                           (int)(mUnbeatableGagePercent *
		                                 PLAYER_UNBEATABLE_GAUGE_WIDTH); // 右端
		int unbeatableGageBottomY = (int)mY +
		                            PLAYER_UNBEATABLE_GAUGE_TOP_OFFSET +
		                            PLAYER_UNBEATABLE_GAUGE_HEIGHT; // 下橋
		// ゲージの表示
		DrawBox(unbeatableGageLeftX, unbeatableGageTopY, unbeatableGageRightX,
		        unbeatableGageBottomY, ORANGE_COLLAR, TRUE);
	}
	else // 通常画像を表示
	{
		DrawGraph((int)mX - mImageWidth / CUT_HALF,
		          (int)mY - mImageHeight / CUT_HALF,
		          Data::getInstance()->mPleyerImageHandle, true);
	}

	// HPをアイコンで表示
	for (int i = 0; i < PLAYER_LIFE_ICON_AMOUNT; i++)
	{
		if (mIsActiveLifeIcon[i]) // 残りHPに応じて表示アイコンを管理
		{
			// HPアイコンの表示
			// プレイヤーの下の位置に表示
			// ループ回数によって位置をずらす
			DrawGraph((int)mX + PLAYER_LIFE_ICON_BASE_X_OFFSET -
			              PLAYER_LIFE_ICON_SPACING_X *
			                  (PLAYER_LIFE_ICON_INDEX_MAX - i),
			          (int)mY + PLAYER_LIFE_ICON_Y_OFFSET,
			          Data::getInstance()->mLifeItemImageHandle, TRUE);
		}
	}
}

/*
@brief	動作開始時に位置やフラグなど各パラーメータの初期設定を行う関数

@param	なし
@return		なし

*/
void Player::start()
{
	mX = SCREEN_CENTER_X;
	mY = SCREEN_CENTER_Y;
	mSpeed = PLAYER_SPEED;
	mLife = MAX_LIFE;
	mIsActiveMissileShot = false;
	mIsTakeDamage = false;
	mIsDefeat = false;
	mDamageDisplayCount = 0;
	mShotPower = 0;
	mIsUnbeatable = false;
	mUnbeatableCount = 0;
	mUnbeatableGagePercent = PLAYER_UNBEATABLE_MAX_PERCENT;
	mWidth = PLAYER_HIT_RANGE;
	mHeight = PLAYER_HIT_RANGE;
	mIsActive = true;

	for (int i = 0; i < MAX_HP; i++)
	{
		mIsActiveLifeIcon[i] = TRUE;
	}
	mIsDamegeCoolDown = false;
	mIsItemHit = false;

	mNomalShotIntervalCount = 0;
	mNomalShotInterval = PLAYER_NORMAL_SHOT_SPEED;
	mMissileShotIntervalCount = 0;
	mSpecialShotCount = 0;
	mShotCount = 0;
	mHitItemType = 0;
}

/*
@brief	プレイヤーの弾発射を管理をする関数

@param[in]	BulletManager *bulletManager		:弾管理クラスのポインタ
@param[in]	EffectManager *effectManager		:エフェクト管理クラスのポインタ

@return		なし

@note

      - スペースキーを押しっぱなしの場合、弾を連続で発射している
      - 発射パターン変化時に強化SEの再生と強化エフェクトの表示を行う
      - 次の変数でカウントを行い、処理を管理している
        - mNomalShotIntervalCount			通常弾の発射間隔を管理
        - mMissileShotIntervalCount			ミサイル弾発射間隔を管理
        - mSpecialShotCount　　				スペシャル弾の発射間隔を管理4
        - mShotCount　						共通の発射間隔を管理
      　
      - ショットパワーに応じて発射パターンが強化されている

        1. ショットパワー1500以下
           - 一定間隔でプレイヤー通常弾の発射を繰り返す

        2. ショットパワー1500以上
           -  発射間隔を短くして一定間隔でプレイヤー通常弾の発射を繰り返す

        3. ショットパワー3000以上
           -  間隔を短くして一定間隔でプレイヤー通常弾の発射を繰り返す
           - ミサイル弾の発射を追加
             -左右同時に発射を繰り返している。

        4. ショットパワー6000以上、　
           - 間隔を短くして一定間隔でプレイヤー通常弾の発射を繰り返す
           - ミサイル弾の発射を追加
             - 左右同時に発射を繰り返している
           - スペシャル弾の追加
             - スペシャル弾の扇状の弾幕を一定間隔で繰り返し発射
             - 5方向にスペシャル弾を同時に発射
             - 5連続で弾幕を発射

*/

void Player::shotPlayerBullet(BulletManager *bulletManager,
                              EffectManager *effectManager)
{
	// 発射カウントの更新
	mShotCount++;
	// 5フレーム間隔で処理を実行
	if (mShotCount > PLAYER_NOMAL_SHOT_INTERVAL)
	{
		mShotCount = 0; // 発射カウントのリセット
		// スペースキーを押されると発射
		if (checkPushkey(KEY_INPUT_SPACE))
		{
			mNomalShotIntervalCount++; // 発射間隔カウントの更新
			// 発射間隔カウントの2カウント間隔で実行(実質10フレーム
			if (mNomalShotIntervalCount % mNomalShotInterval == 0)
			{
				mNomalShotIntervalCount = 0;
				for (int i = 0; i < PLAYER_MISSILE_AMOUNT; i++)
				{

					bulletManager->setBullet(
					    (int)mX - FIRE_POINT_OFFSET_X +
					        i * (FIRE_POINT_OFFSET_X * 2),
					    (int)mY + FIRE_POINT_OFFSET_Y, PLAYER_NORMAL_SHOT_ANGLE,
					    BULLET_TYPE::PLAYER_NOMAL, false, true);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mPlayerShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);

			}
			// shootPowarが1500超えたら発射間隔を短くする
			if (mShotPower >= SHOT_POWER_GRADE1 &&
			    mNomalShotInterval == PLAYER_NORMAL_SHOT_SPEED)
			{
				// 強化SEの再生
				PlaySoundMem(Data::getInstance()->mPowerUpSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
				effectManager->setEffect(&(mX), &(mY), POWERUP_EF);

				mNomalShotInterval =
				    PLAYER_UPGRADE_SHOT_SPEED; // 発射間隔カウントの1カウント間隔で実行（実質5フレーム）
			}

			// shootPowarが３０００超えたらミサイル弾追加
			// ミサイル発射フラグで1度だけ実行
			if (mShotPower >= SHOT_POWER_GRADE2 &&
			    mIsActiveMissileShot == false)
			{
				// 強化SEの再生
				PlaySoundMem(Data::getInstance()->mPowerUpSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
				effectManager->setEffect(&(mX), &(mY), POWERUP_EF);

				// ミサイル弾発射フラグをオン
				mIsActiveMissileShot = true;
			}
			// ミサイル弾発射フラグがオンの場合
			if (mIsActiveMissileShot)
			{
				// ミサイル弾発射カウントの更新
				mMissileShotIntervalCount++;
				if (mMissileShotIntervalCount % PLAYER_MISSILE_SHOT_INTERVAL ==
				    0)
				{
					mMissileShotIntervalCount =
					    0; // ミサイル弾発射カウントのリセット

					// 2発分発射を繰り返す
					// 左斜め上、右斜め上方向に向けて発射
					for (int i = 0; i < PLAYER_MISSILE_AMOUNT; i++)
					{

						bulletManager->setBullet(
						    (int)mX - FIRE_POINT_OFFSET_X +
						        i * (FIRE_POINT_OFFSET_X * 2),
						    (int)mY + FIRE_POINT_OFFSET_Y,
						    i * PLAYER_MISSILE_ANGLE_STEP +
						        PLAYER_MISSILE_START_ANGLE + TURN_AROUND_ANGLE,
						    BULLET_TYPE::PLAYER_MISSILE, false, true);
					}
					// 発射SEの再生
					PlaySoundMem(
					    Data::getInstance()->mPlayerShotSoundEffectHandle,
					    DX_PLAYTYPE_BACK, TRUE);

				}

				// カウントのオーバフロー防止
				if (mMissileShotIntervalCount >=
				    PLAYER_MISSILE_SHOT_COUNTER_RESET)
				{
					mMissileShotIntervalCount = 0;
				}
			}
			if (mShotPower == SHOT_POWER_GRADE3) // 一度だけ処理を実行させる
			{
				// 強化SEの再生
				PlaySoundMem(Data::getInstance()->mPowerUpSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
				effectManager->setEffect(&(mX), &(mY), POWERUP_EF);
			}

			// shootPowarが６０００超えたらスペシャル使用可能
			if (mShotPower >= SHOT_POWER_GRADE3)
			{
				// スペシャル弾発射カウントの更新
				mSpecialShotCount++;

				// 5カウントまで発射を繰り返す
				// 発射間隔は実質25フレーム
				if (mSpecialShotCount < PLAYER_SPECIAL_SHOT_ACTIVE_COUNT)
				{
					// 5発分発射を繰り返す。同時発射
					// スペシャル弾の扇状の弾幕
					for (int i = 0; i < PLAYER_SPECIAL_AMOUNT; i++)
					{
						bulletManager->setBullet(
						    (int)mX, (int)mY,
						    i * SPECIAL_BULLET_ANGLE_STEP +
						        SPECIAL_BULLET_BASE_ANGLE -
						        (SPECIAL_BULLET_ANGLE_STEP *
						         SPECIAL_BULLET_SPREAD_COUNT) /
						            CUT_HALF,
						    BULLET_TYPE::PLAYER_SPECIAL, false, true);
					}
					// 発射SEの再生
					PlaySoundMem(
					    Data::getInstance()->mPlayerShotSoundEffectHandle,
					    DX_PLAYTYPE_BACK, TRUE);

				}
				// 10カウントでスペシャル弾発射カウントをリセット(実質50フレーム)
				if (mSpecialShotCount > PLAYER_SPECIAL_SHOT_COUNTER_RESET)
				{
					mSpecialShotCount = 0;
				}
			}
		}
	}
}
/*
@brief	ダメージ処理を行う関数

@param	なし

@return	なし
@note
- ダメージフラグ（mIsTakeDamage）がfalseの場合処理を実行しない
- 加えて無敵フラグ(mIsUnbeatable)とダメージ処理インターバルフラグ（mIsDamegeCoolDown）がfalseの場合は処理を実行する
  - ダメージSEの再生
  - ヒットポイントの減算
mLifeが０以下の場合は撃破処理（フラグ更新）を行う

*/
void Player::takeDamage()
{
	if (!mIsTakeDamage) // ダメージ判定がない場合実行しない
	{
		return;
	}
	mIsTakeDamage = false; // ダメージフラグをfalse
	if (mIsUnbeatable ||
	    mIsDamegeCoolDown) // ダメ処理インタバール中もしくは無敵の場合もダメージ処理は実行しない
	{
		return;
	}
	// ダメージ処理
	mLife--;                          // HPを減らす
	mIsActiveLifeIcon[mLife] = false; // 表示アイコン用のフラグをfalse
	PlaySoundMem(Data::getInstance()->mPlayerDamageSoundEffectHandle,
	             DX_PLAYTYPE_BACK, TRUE); // ダメージSEの再生
	mIsDamegeCoolDown = true;             // ダメージ無効フラグをtrue

	if (mLife <= 0) // 撃破判定
	{
		mIsDefeat = true; // 撃破フラグのtrue
		mIsActive = false;
	}
}

/*
@brief	各アイテムオブジェクトの効果反映処理を行う関数

@param[in]	EffectManager *effectManager :エフェクト管理クラスのポインタ
@param[in]	int*    score		:ゲームスコアのポインタ

@return		当たったアイテムの種類:int
@note  経験値アイテム　OBJECT_EXP  1
@note  回復アイテム　OBJECT_LIFE 2
@note  無敵アイテム　OBJECT_STAR 3
@note

- アイテムオブジェクトに応じて処理を分岐する

    1. 経験値アイテム
        - ショットパワーを増やす

    2. 回復アイテム
        - 残機を1つ増やす
        - 残機が最大の場合は増やさない

    3. 無敵アイテム
        - 無敵にする
        - 無敵の場合は残り時間を最大値まで戻す
*/
void Player::applyItemEffect(int *score, EffectManager *effectManager)
{
	if (!mIsItemHit)
	{
		return;
	}
	switch (mHitItemType)
	{
	case OBJECT_EXP: // 経験値アイテム
		PlaySoundMem(Data::getInstance()->mEXPItemGetSoundEffectHandle,
		             DX_PLAYTYPE_BACK, TRUE); // アイテム獲得SEの再生

		(score) += EXP_SCORE;                    // 経験値スコアの加算
		mShotPower += SHOT_POWER_INCREASE_AMONT; // ショットパワーが加算

		break;

	case OBJECT_LIFE: // 回復アイテム
		PlaySoundMem(Data::getInstance()->mLifeItemGetSoundEffectHandle,
		             DX_PLAYTYPE_BACK, TRUE); // アイテム獲得SEの再生

		effectManager->setEffect(&(mX), &(mY), LIFE_EF); // 獲得エフェクトの再生

		(score) += LIFE_SCORE; // アイテム獲得スコアの加算
		// 最大の場合効果なし
		if (mLife != MAX_HP)
		{
			// HPを1つ回復
			mIsActiveLifeIcon[mLife] = true; // 表示アイコン用のフラグをtrue
			mLife++;
		}
		break;

	case OBJECT_STAR: // 無敵アイテム
		PlaySoundMem(Data::getInstance()->mUnbeatableItemGetSoundEffectHandle,
		             DX_PLAYTYPE_BACK, TRUE); // アイテム獲得SEの再生

		effectManager->setEffect(&(mX), &(mY), STAR_EF); // 獲得エフェクトの再生

		(score) += STAR_SCORE; // アイテム獲得スコアの加算
		// 無敵中の場合、効果時間を最大値にリセット
		if (!mIsUnbeatable)
		{
			mIsUnbeatable = true;
		}
		else
		{
			mUnbeatableCount = 0;
		}
		break;

	default:
		break;
	}
	// アイテムヒット判定をfalse。ヒットアイテム種類のリセット。
	mIsItemHit = false;
	mHitItemType = 0;
}
