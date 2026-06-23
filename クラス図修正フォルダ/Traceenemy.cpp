#include "Traceenemy.h"
/*
@brief	コンストラクタ

@param	　　なし
@return		なし

@note      各種初期設定。クラス生成の際に必要なパラメータはすべて設定する

*/
TraceEnemy::TraceEnemy()
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
TraceEnemy::~TraceEnemy()
{
}

/*
@brief	ゲームシーンのメインループ内で実行する更新処理を行う関数

@param[in] int playerX:プレイヤーのX座標
@param[in] int playerY:プレイヤーのY座標

@return		なし

@note

- フラグ(FLG)がtrueの場合のみ処理を実行する
- MoveCount を用いて移動タイミングや動作を制御している
- ステージに応じて処理を分岐している

1. 通常ステージ
    - 出現してからy=100の位置までは降下移動をする
    - 降下移動後はプレイヤーを追いかける移動をする
    - 画面外へ行くとフラグ(FLG)をfalseにし撃破フラグ(Defeat)をtrueにする

2. ボスステージ
    - ボスに召喚されて指定位置まで移動する
    - 指定位置に移動後、一定カウント内までに倒されなかった場合、画面外へ移動する

*/
void TraceEnemy::action(int playerX, int playerY, int *score)
{
	if (!mIsActive)
	{
		return;
	}
	takeDamage(score); // ダメージ処理
	mMoveCount++;      // 移動カウントの更新

	if (!mIsBossStage)
	{
		if (mMoveCount % MOVE_INTERVAL_FRAME == 0) // ２フレームに一回移動
		{
			// y100まで降下
			if (mY >= ENEMY_DESCEND_LIMIT_Y)
			{
				mIsReachedStartPosition = true;
			}

			if (mIsReachedStartPosition == false)
			{
				mVectorY *= ENEMY_SLOWDOWN_Y; // 徐々に減速
				mY += mVectorY;
			}
			else // プレイヤーを追いかけるように移動
			{
				// atan2を使用してプレイヤーと追従敵とのなす角を計算
				double vectorTargetX = (playerX - mX);
				double vectorTargetY = (playerY - mY);
				double targetAngle = atan2(vectorTargetY, vectorTargetX);
				// 角度から速度を計算
				mVectorX = cos(targetAngle) * TRACE_ENEMY_TRACK_SPEED;
				mVectorY = sin(targetAngle) * TRACE_ENEMY_TRACK_SPEED;
				// 位置(X,Y)に速度(Vx,Vy)を加算して、移動している
				mX += mVectorX;
				mY += mVectorY;
			}
			// 画面外なら消える処理
			if (mX >= MAX_SCREEN_WIDTH + mWidth || mX <= -(mWidth) ||
			    mY >= MAX_SCREEN_HEIGHT + mHeight || mY <= -(mHeight))
			{
				mIsActive = false;
				mIsDefeat = true;
			}
		}
	}
	else
	{
		if (mNumber == SUMMON_LEFT) // 左側
		{
			// ポジションにつくまで移動
			// 指定位置との誤差が一定値になるまで移動処理を実行
			if (abs((int)mX - TRACE_ENEMY_SUMMON_POINT_LEFT_X) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    abs((int)mY - TRACE_ENEMY_SUMMON_POINT_LEFT_Y) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    !mIsReachedBossPosition)
			{
				// atan2を使用してプレイヤーと追従敵とのなす角を計算
				double vectorTargetX = (TRACE_ENEMY_SUMMON_POINT_LEFT_X - mX);
				double vectorTargetY = (TRACE_ENEMY_SUMMON_POINT_LEFT_Y - mY);
				double targetAngle = atan2(vectorTargetY, vectorTargetX);
				// 角度から速度を計算
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
			// ポジションにつくまで移動
			// 現在の位置と指定位置のずれが一定値になるまで処理を実行する
			if (abs((int)mX - TRACE_ENEMY_SUMMON_POINT_RIGHT_X) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    abs((int)mY - TRACE_ENEMY_SUMMON_POINT_RIGHT_Y) >=
			        ENEMY_POSITION_DIFFERENCE_RANGE &&
			    !mIsReachedBossPosition)
			{
				// atan2を使用して敵と指定位置とのなす角を計算
				double vectorTargetX = (TRACE_ENEMY_SUMMON_POINT_RIGHT_X - mX);
				double vectorTargetY = (TRACE_ENEMY_SUMMON_POINT_RIGHT_Y - mY);
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
	// オーバーフロー防止
	if (mMoveCount == MOVE_COUNT_LIMIT)
	{
		mMoveCount = 0; // 移動カウントのリセット
	}
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note      フラグ(FLG)がtrueの場合のみ処理を実行する
@note      ダメージフラグ(dmgFlg)がtrueの場合はダメージ用の画像を表示する
@note      なにもなければ、通常の追従敵の画像を表示する

*/
void TraceEnemy::draw()
{
	if (mIsActive)
	{
		// ダメージ時の場合
		if (mIsDamageCoolDown)
		{
			mDamageDisplayCount++; // ダメージ表示カウントの更新
			// ダメージ画像の表示
			// マジックナンバー
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
			// マジックナンバー
			DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
			          Data::getInstance()->mTraceEnemyImageHandle, TRUE);
		}
	}
}

/*
@brief	動作開始時に位置・体力・フラグなど各パラーメータの初期設定を行う関数

@param	なし
@return		なし

@note      追従敵の生成時や、状態を初期状態に戻す際に使用される。

*/
void TraceEnemy::start()
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
