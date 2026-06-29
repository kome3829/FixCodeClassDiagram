#include "BossEnemy.h"
#include "BulletManager.h"
#include "EffectManager.h"
/*
@brief	コンストラクタ

@param[in]	なし
@return		なし

@note	各種初期設定。クラス生成の際に必要なパラメータはすべて設定する
@note	ゴミ値が入らないように変数の初期化を行う
@note	クラス内で使用する別クラスのインスタンスの生成を行う
*/

BossEnemy::BossEnemy()
{
	mX = MAX_SCREEN_WIDTH / CUT_HALF;
	mY = 0;
	mWidth = ENEMY_BOSS_WIDTH;
	mHeight = ENEMY_BOSS_HEIGHT;
	mHitPoint = BOSS_MAX_HP;
	mVectorX = BOSS_ENEMY_VECTOR_X;
	mVectorY = BOSS_ENEMY_VECTOR_Y;
	mSpeed = BOSS_ENEMY_SPD;
	mIsReachedStartPosition = false;

	mIsActive = false;
	mIsDropExperiencePoint = false;
	mMoveCount = 0;
	mIsDefeat = false;
	mNumber = 0;
	mShotCount = 0;
	mAlpha = ALPHA_MAX;
	mBossHitPointPercent = BOSS_ENEMY_MAX_HP_PERCENT;
	mShotBulletCount = 0;
	mBossDirection = RIGHT;
	mShotMiniBulletCount = 0;
	mBossHitPointBarRightEnd = 0;
	mCentralPositionAngle = 0;
	mCenterPositionVectorX = 0;
	mCenterPositionVectorY = 0;
	mMovementVectorX = 0;
	mMovementVectorY = 0;
	mHasSetSpeed = false;
	mIsUnbeatable = false;
	mIsResetMoveCount = false;
	mIsSummonEnemy = false;
	for (int i = 0; i < BOSS_PATTARN_MAX; i++)
	{
		mIsExecuteProcess[i] = FALSE;
	}
	mMoveTargetAngle = 0;
	mIsWarp = false;
	mIsTakeDamage = false;
}

/*
@brief	デストラクタ

@param[in]	なし
@return		なし

@note	クラス内メンバは自動で解放されるので特に記載なし
@note	※コンストラクタで生成したインスタンスのメモリ開放を行う必要がある
*/
BossEnemy::~BossEnemy()
{
}

/*
@brief	メインループで実行する更新処理を行う関数

@param[in]	なし
@return		なし

@note	HP計算を行う
@note	ボス敵の行動フラグ(FLG)がtureの場合のみ処理を実行する
@note	行動を体力応じて分岐している
@note	ボス敵の行動の種別は体力応じて、5つある
@note	行動を切り替え間の処理は３つ
@note	撃破フラグに応じて撃破表示の更新処理を行う

@warning
- 余裕のある行動から、激しい行動までを体力応じて行動パターンを作成した
- 行動処理を分岐させるために必要なHP計算をはじめに行う
*/
void BossEnemy::action(int *score, BulletManager *bulletManager,
                       EffectManager *effectManager,
                       ItemObjectManager *itemObjectManager,
                       MinionEnemyManager *minionEnemyManager, Player *player)
{
	// HP計算
	if (mHitPoint > 0)
	{
		// ボスの現在HPをパーセントに変換
		mBossHitPointPercent = (int)((mHitPoint / (double)BOSS_MAX_HP) *
		                             BOSS_ENEMY_MAX_HP_PERCENT);
		// HPバーの横幅を計算
		mBossHitPointBarRightEnd = BOSS_HP_BER_MAXWIDTH * mBossHitPointPercent /
		                           BOSS_ENEMY_MAX_HP_PERCENT;
	}
	takeDamage(score); // ダメージ処理
	shotBossEnemyBullet(bulletManager, effectManager, itemObjectManager,
	                    minionEnemyManager, player);
	// boss動き、体力応じて分岐
	// 分岐の値はわかりやすいため、ラベル適宜で置き換えずそのままにしている
	if (mIsActive && mIsDefeat == false)
	{
		// マジックナンバー
		if (mBossHitPointPercent <= BOSS_HP_PER_MAX &&
		    mBossHitPointPercent > BOSS_HP_PER_PHASE1_END)
		{
			moveLeftAndRight();
		}
		else if (mBossHitPointPercent == BOSS_HP_PER_PHASE1_END)
		{
			settingNextPatrolMove();
		}
		else if (mBossHitPointPercent < BOSS_HP_PER_PHASE1_END &&
		         mBossHitPointPercent > BOSS_HP_PER_PHASE2_END)
		{
			patrolMoveFourPoint();
		}
		else if (mBossHitPointPercent == BOSS_HP_PER_PHASE2_END)
		{
			settingNextWarpMove();
		}
		else if (mBossHitPointPercent < BOSS_HP_PER_PHASE2_END &&
		         mBossHitPointPercent > BOSS_HP_PER_PHASE3_END)
		{
			warpMoveFourPoint(effectManager);
		}
		else if (mBossHitPointPercent == BOSS_HP_PER_PHASE3_END)
		{
			settingNextWarpAndAttackMove();
		}
		else if (mBossHitPointPercent < BOSS_HP_PER_PHASE3_END &&
		         mBossHitPointPercent > BOSS_HP_PER_DEFEAT)
		{
			warpAndAttackMove(effectManager);
		}
		else if (mBossHitPointPercent == BOSS_HP_PER_DEFEAT)
		{
			moveDefeat();
		}
	}
	// 撃破された場合実行
	if (mIsDefeat)
	{
		// 撃破演出の透過度の更新処理
		// ゼロになるまで減算
		mAlpha--;
		if (mAlpha <= 0)
		{
			mAlpha = 0;
		}
	}
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param[in]	なし
@return		なし

@note	ボス敵の行動フラグ(FLG)がtrueの場合のみ処理を実行する
@note	撃破時には徐々に透明になっていく演出を行う
@note	ダメージを受けていたら、ダメージ用画像表示をする
@note	どの状態でもなければ通常の表示する
@note	エフェクトの表示。攻撃や行動時に合わせて演出として使用する
@warning
-
撃破時の徐々に透明になっていく演出でこれまでとは違う強い敵を撃破したことを伝える
*/
void BossEnemy::draw()
{
	if (!mIsActive)
	{
		return;
	}
	// 撃破された場合
	if (mIsDefeat)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, mAlpha);
		DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
		          Data::getInstance()->mBossImageHandle,
		          TRUE);                           // 透過度を反映させた画像表示
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 透過度のリセット
	}
	else if (mIsWarp)//ワープ演出中の場合
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, mAlpha);
		DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
		          Data::getInstance()->mBossDamageImageHandle,
		          TRUE);                           // ダメージ用画像の表示
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 透過度のリセット

	}
	else
	{
		// ダメージを受けた場合
		if (mIsDamageCoolDown)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, mAlpha);
			DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
			          Data::getInstance()->mBossDamageImageHandle,
			          TRUE);                           // ダメージ用画像の表示
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 透過度のリセット

			// カウントを行い、ダメージ画像の表示フレームを制限する
			mDamageDisplayCount++;
			if (mDamageDisplayCount > DAMAGE_COUNT_LIMIT)
			{
				mIsDamageCoolDown = false;
				mDamageDisplayCount = 0;
			}
		}
		else
		{
			// 通常表示
			DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
			          Data::getInstance()->mBossImageHandle, TRUE);
		}
	}
}

/*
@brief	動作開始するときに必要なパラーメータの初期設定や処理を行う関数

@param[in]	なし
@return		なし

@note	位置やフラグなどの初期値の設定が必要なパラーメータの設定を行う
*/
void BossEnemy::start()
{
	mMoveTargetAngle = 0;
	mAlpha = ALPHA_MAX;
	mBossHitPointPercent = BOSS_ENEMY_MAX_HP_PERCENT;
	mX = MAX_SCREEN_WIDTH / CUT_HALF;
	mY = 0;
	mWidth = ENEMY_BOSS_WIDTH;
	mHeight = ENEMY_BOSS_HEIGHT;
	mHitPoint = BOSS_MAX_HP;
	mVectorX = BOSS_ENEMY_VECTOR_X;
	mVectorY = BOSS_ENEMY_VECTOR_Y;
	mSpeed = BOSS_ENEMY_SPD;
	mIsReachedStartPosition = false;
	mIsActive = false;
	mMoveCount = 0;
	mIsDefeat = false;
	mNumber = 0;
	mShotCount = 0;
	mBossDirection = RIGHT;
	mShotBulletCount = 0;
	mIsDropExperiencePoint = false;
	mShotMiniBulletCount = 0;
	mBossHitPointBarRightEnd = 0;
	mCentralPositionAngle = 0;
	mCenterPositionVectorX = 0;
	mCenterPositionVectorY = 0;
	mMovementVectorX = 0;
	mMovementVectorY = 0;
	mHasSetSpeed = false;
	mIsUnbeatable = false;
	mIsResetMoveCount = false;
	mIsSummonEnemy = false;

	for (int i = 0; i < BOSS_PATTARN_MAX; i++)
	{
		mIsExecuteProcess[i] = FALSE;
	}
	mIsWarp = false;
	mIsTakeDamage = false;
}

/*
@brief	メインループで動作させるボス敵のヒット判定を行う関数

@param[in]	Bullet* bullet : 弾クラスインスタンスのポインタ
@param[in]	MissileBullet* missileBullet :
ミサイル弾クラスインスタンスのポインタ
@param[in]	SpecialBullet* specialBullet :
スペシャル弾クラスインスタンスのポインタ
@param[in]	int* score : ゲームスコアのポインタ

@return		なし

@note	撃破フラグ(Defeat)がfalseの場合のみ処理を実行する
@note	プレイヤーの弾がボス敵にヒットしているかを判定する
@note
弾の種別によって分岐し、ボス敵と弾の位置を計算して弾が一定範囲内なら当たり判定を返す
@note	判定に応じてダメージ処理、スコア加算、撃破判定を行う
*/

/*
@brief	ボス敵を初期位置へ移動させる行動を行う関数

@param	    なし
@return		実行状態:bool

@note   実行中 false /完了 true
@note   ボス敵の行動パターンの切り替え時に使用する
@note	位置についたか判定も行うため返り値をbool型としている
@warning
- 次の行動のためにボスの位置を画面中央上へ戻す

*/
bool BossEnemy::moveCenterPosition()
{
	// 速度が設定されていない場合のみ。設定する
	if (!mHasSetSpeed)
	{
		mSpeed = BOSS_ENEMY_SPD;
		mHasSetSpeed = true;
	}
	// 逆三角関数(atan2)を使用し、移動させたい位置とボス敵の位置とのなす角を計算し、
	// 角度から速度（Vx,Vy）を計算し設定している
	mCenterPositionVectorX = SCREEN_CENTER_X - mX;
	mCenterPositionVectorY = BOSS_CENTER__POSITION_Y - mY;
	mCentralPositionAngle =
	    atan2(mCenterPositionVectorY, mCenterPositionVectorX);
	mVectorX = cos(mCentralPositionAngle) * mSpeed;
	mVectorY = sin(mCentralPositionAngle) * mSpeed;
	// 指定位置と等しくなるまで位置に速度を足して移動する
	if (mX != SCREEN_CENTER_X)
	{
		mX += mVectorX;
	}
	if (mY != BOSS_CENTER__POSITION_Y)
	{
		mY += mVectorY;
	}
	// ボス敵と画面中央上の座標ずれが許容範囲内になるまで移動を実行する
	if (abs(SCREEN_CENTER_X - (int)mX) < POSITION_TOLERANCE &&
	    abs(BOSS_CENTER__POSITION_Y - (int)mY) < POSITION_TOLERANCE)
	{
		return true;
	}
	return false;
}

/*
@brief action関数内で使用するボス敵の行動処理（HP100～75%段階）

@param[in] なし
@return    なし

@note
画面上部を左右に往復移動する。
進行方向に応じて移動処理を分岐している。

@note
HPが100～75%のときにこの行動へ遷移する。

@warning
- 序盤フェーズのため、プレイヤーが狙いやすいよう
単純な移動パターンにしている
*/
void BossEnemy::moveLeftAndRight()
{
	// 降下移動　y＝40の位置まで
	// 修正案。なぜ画像サイズの半分足しているのか必要があるのか調べる
	if (mY <= mHeight / CUT_HALF + BOSS_DESCEND_LIMIT_Y)
	{
		mVectorY *= BOSS_ENEMY_SLOWDOWN_Y; // 徐々に減速
		mY += mVectorY;
	}
	// 左右往復移動
	else
	{
		switch (mBossDirection) // 進行方向によって分岐
		{

		case LEFT: // 左移動
			mX -= mVectorX;
			// 左端まで移動した場合
			if (mX <= mWidth / CUT_HALF)
			{
				mBossDirection = RIGHT; // 移動方向を右に切り替え
			}
			break;

		case RIGHT: // 右移動
			mX += mVectorX;
			// 右端まで移動した場合
			if (mX >= MAX_SCREEN_WIDTH - mWidth / CUT_HALF)
			{
				mBossDirection = LEFT; // 移動方向を左に切り替え
			}
			break;

		default:
			break;
		}
	}
}

/*
@brief	action関数で動作させるボス敵の行動の関数(HPが７４～５０％段階)
@param[in]	なし
@return		なし

@note	HPが７４～５０％の場合この関数へ分岐
@note	４つのポイント座標を巡回する移動
@note   MoveCount変数でカウントし、処理を管理
@warning
- 大きくボス敵の位置を変える事でプレイヤーの移動操作を促す
*/
void BossEnemy::patrolMoveFourPoint()
{
	mMoveCount++; // 移動カウントの更新
	// 位置に速度を加算して移動
	mX += mVectorX;
	mY += mVectorY;
	// ボス敵の向かう方向に応じて処理を分岐
	switch (mBossDirection)
	{
	case POINT_LEFT: // 左側の位置
		// 移動カウントが15フレームごとに処理を実行
		if (mMoveCount % BOSS_MOVE_INTERVAL_FRAME == 0)
		{
			// 一定値まで徐々にスピードダウン
			if (mSpeed > BOSS_ENEMY_SPD_DOWN_LIMIT)
			{
				mSpeed *= BOSS_ENEMY_SLOWDOWN;
			}
			// 角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
		}
		// ボス敵と移動させたい位置の座標ずれが許容範囲内になるまで実行する
		// 修正案。y座標の条件式< POSITION_TOLERANCEが抜けていると思われる
		if (abs(BOSS_WARP_LEFT_POINT_X - (int)mX) < POSITION_TOLERANCE &&
		    abs(BOSS_WARP_LEFT_POINT_Y - (int)mY))
		{
			// atan2を使用して次に向かう位置から角度を計算
			mMovementVectorX = BOSS_WARP_BOTTOM_POINT_X - mX;
			mMovementVectorY = BOSS_WARP_BOTTOM_POINT_Y - mY;
			mMoveTargetAngle = atan2(mMovementVectorY, mMovementVectorX);
			// 角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
			mSpeed = BOSS_ENEMY_SPD; // 初速を設定
			mBossDirection =
			    POINT_BOTTOM; // 向かう方向を画面の下の位置に切り替え
		}
		break;

	case POINT_BOTTOM: // 下の位置
		// 移動カウントが15フレームごとに処理を実行
		if (mMoveCount % BOSS_MOVE_INTERVAL_FRAME == 0)
		{
			// 一定値まで徐々にスピードダウン
			if (mSpeed > BOSS_ENEMY_SPD_DOWN_LIMIT)
			{
				mSpeed *= BOSS_ENEMY_SLOWDOWN;
			}
			// 角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
		}
		// ボス敵と移動させたい位置の座標ずれが許容範囲内になるまで実行する
		// 修正案。y座標の条件式< POSITION_TOLERANCEが抜けていると思われる
		if (abs(BOSS_WARP_BOTTOM_POINT_X - (int)mX) < POSITION_TOLERANCE &&
		    abs(BOSS_WARP_BOTTOM_POINT_Y - (int)mY))
		{
			// atan2を使用して次に向かう位置から角度を計算
			mMovementVectorX = BOSS_WARP_RIGHT_POINT_X - mX;
			mMovementVectorY = BOSS_WARP_RIGHT_POINT_Y - mY;
			mMoveTargetAngle = atan2(mMovementVectorY, mMovementVectorX);
			// 角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
			mBossDirection =
			    POINT_RIGHT;         // 向かう方向を画面の右の位置に切り替え
			mSpeed = BOSS_ENEMY_SPD; // 初速を設定
		}
		break;

	case POINT_RIGHT: // 右の位置
		// 一定フレーム毎で実行
		if (mMoveCount % BOSS_MOVE_INTERVAL_FRAME == 0)
		{
			// 一定値まで徐々にスピードダウン
			if (mSpeed > BOSS_ENEMY_SPD_DOWN_LIMIT)
			{
				mSpeed *= BOSS_ENEMY_SLOWDOWN;
			}
			// 角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
		}
		// ボス敵と移動させたい位置の座標ずれが許容範囲内になるまで実行する
		// 修正案。y座標の条件式< POSITION_TOLERANCEが抜けていると思われる
		if (abs(BOSS_WARP_RIGHT_POINT_X - (int)mX) < POSITION_TOLERANCE &&
		    abs(BOSS_WARP_RIGHT_POINT_Y - (int)mY))
		{
			// atan2を使用して次に向かう位置から角度を計算
			mMovementVectorX = BOSS_WARP_TOP_POINT_X - mX;
			mMovementVectorY = BOSS_WARP_TOP_POINT_Y - mY;
			mMoveTargetAngle = atan2(mMovementVectorY, mMovementVectorX);
			// 角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
			mBossDirection = POINT_TOP; // 向かう方向を画面の上の位置に切り替え
			mSpeed = BOSS_ENEMY_SPD;    // 初速を設定
		}
		break;

	case POINT_TOP: // 上の位置
		// 一定フレーム毎で実行
		if (mMoveCount % BOSS_MOVE_INTERVAL_FRAME == 0)
		{
			// 一定値まで徐々にスピードダウン
			if (mSpeed > BOSS_ENEMY_SPD_DOWN_LIMIT)
			{
				mSpeed *= BOSS_ENEMY_SLOWDOWN;
			}
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
		}
		// ボス敵と移動させたい位置の座標ずれが許容範囲内になるまで実行する
		// 修正案。y座標の条件式< POSITION_TOLERANCEが抜けていると思われる
		if (abs(BOSS_WARP_TOP_POINT_X - (int)mX) < POSITION_TOLERANCE &&
		    abs(BOSS_WARP_TOP_POINT_Y - (int)mY))
		{
			// 次に向かう方向、と角度、初速を設定
			mMovementVectorX = BOSS_WARP_LEFT_POINT_X - mX;
			mMovementVectorY = BOSS_WARP_LEFT_POINT_Y - mY;
			mMoveTargetAngle = atan2(mMovementVectorY, mMovementVectorX);
			// 角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
			mBossDirection = POINT_LEFT; // 向かう方向を画面の左位置に切り替え
			mSpeed = BOSS_ENEMY_SPD;     // 初速を設定
		}
		break;

	default:
		break;
	}
}

/*
@brief	action関数で動作させるボス敵の行動の関数(HPが４９～２５％段階)
@param[in]	なし
@return		なし

@note	HPが４９～２５％の場合この関数へ分岐
@note   MoveCount変数でカウントし、処理を管理
@note	ワープで４つのポイント座標に巡回する移動
@warning
- ボス敵の急な位置の変化でプレイヤーを脅かす
*/
void BossEnemy::warpMoveFourPoint(EffectManager *effectManager)
{
	mMoveCount++; // 移動カウントの更新
	// 0～75フレーム間は左へ移動
	if (mMoveCount <= MOVE_PHASE1_END && mMoveCount >= 0)
	{
		// 左移動
		mX--;
		if (mMoveCount <= SHOT_STOP_INTERVAL) // 弾発射を40フレームだけ止める
		{
			mShotBulletCount = 0;
			mShotMiniBulletCount = 0;
		}
	}
	// 75～225フレーム間は右へ移動
	else if (mMoveCount <= MOVE_PHASE2_END && mMoveCount > MOVE_PHASE1_END)
	{
		// 右移動
		mX++;
	}
	// 225～300フレーム間はもう一度左へ移動
	else if (mMoveCount <= MOVE_PHASE3_END && mMoveCount > MOVE_PHASE2_END)
	{
		// 再度左移動
		mX--;
	}
	// 300～360フレーム間はワープまでの待機時間
	// ワープ前の事前に必要な処理を行う
	else if (mMoveCount <= MOVE_PHASE4_END && mMoveCount > MOVE_PHASE3_END)
	{
		// ダメージ画像を表示するため,ダメージフラグをtrue
		mIsWarp = true;
		// 弾の発射カウントのリセット
		mShotBulletCount = 0;
		mShotMiniBulletCount = 0;
		// ワープ演出開始
		if (mMoveCount == WARP_TRIGGER_FRAME_BOSS)
		{
			effectManager->setEffect(&mX, &mY,
			                         WARP_EF);   // ワープエフェクトを表示
			mVectorY = BOSS_ENEMY_WARP_VECTOR_Y; // y軸の速度を変更
		}
		// ワープ演出
		// 演出開始カウントを過ぎているかつ、
		// アルファ値が0以上の場合
		if (mMoveCount > WARP_TRIGGER_FRAME_BOSS && mAlpha > 0)
		{
			// 徐々に薄くなる＆徐々に浮かび上がる
			mAlpha -= FADE_ALPHA_STEP;
			mY += mVectorY;
			mVectorY *= BOSS_ENEMY_SPEEDUP_Y;
		}
	}
	// ワープ実行フレーム(361)
	else if (mMoveCount == MOVE_END_FRAME)
	{
		// ワープ先によって分岐
		switch (mBossDirection)
		{
		case POINT_LEFT: // 画面の左の位置
			// ワープ先に移動
			mX = BOSS_WARP_BOTTOM_POINT_X;
			mY = BOSS_WARP_BOTTOM_POINT_Y;
			mBossDirection = POINT_BOTTOM; // 次のワープ先を設定
			break;
		case POINT_BOTTOM: // 画面の下の位置
			// ワープ先に移動
			mX = BOSS_WARP_RIGHT_POINT_X;
			mY = BOSS_WARP_RIGHT_POINT_Y;
			mBossDirection = POINT_RIGHT; // 次のワープ先を設定

			break;
		case POINT_RIGHT: // 画面の右の位置
			// ワープ先に移動
			mX = BOSS_WARP_TOP_POINT_X;
			mY = BOSS_WARP_TOP_POINT_Y;
			mBossDirection = POINT_TOP; // 次のワープ先を設定

			break;
		case POINT_TOP: // 画面の上の位置
			// ワープ先に移動
			mX = BOSS_WARP_LEFT_POINT_X;
			mY = BOSS_WARP_LEFT_POINT_Y;
			mBossDirection = POINT_LEFT; // 次のワープ先を設定

			break;
		default:
			break;
		}
		// 各カウント変数とアルファ値の初期化
		mShotBulletCount = 0;
		mShotMiniBulletCount = 0;
		mMoveCount = 0;
		mIsWarp = false;
		mAlpha = ALPHA_MAX;
	}
}

/*
@brief	action関数で動作させるボス敵の行動の関数(HPが２４～１％段階)
@param[in]	なし
@return		なし

@note	HPが２４～１％の場合この関数へ分岐
@note   MoveCount変数でカウントし、処理を管理
@note	４つのポイントにワープで巡回する移動と溜め攻撃行う移動を交互に繰り返す
@note	一定カウントで行動を切り替える
@note

    - ワープ移動し溜め攻撃行う移動
        1. 雑魚敵を召喚し一緒に力を溜める
        2. 溜め終わると
画面中央上のポイントへワープ移動して動かずに激しい攻撃を行う

    - ワープで巡回する移動
        - 指定カウントで次のポイントへワープ
        - ワープ移動の合間はそのポイント座標で左右に往復する移動する
        - ボス敵の向かう方向に応じて移動処理を分岐している
@warning
- これまでの移動パターンを組み合わせて、より難易度を難しくする
- 行動パターンを2つ用意することで更に難しくする
*/
void BossEnemy::warpAndAttackMove(EffectManager *effectManager)
{
	if (mMoveCount <= CHARGE_END_FRAME_BOSS) // 240fまでチャージフェーズ
	{
		// 雑魚敵召喚フラグを指定フレームだけでtrue
		if (mMoveCount == SUMMON_ENEMY_SET_FRAME)
		{
			mIsSummonEnemy = true;
		}
		else
		{
			mIsSummonEnemy = false;
		}
		// エフェクトを表示。チャージフェーズ間、再生が終わるごとに再度表示
		if (mMoveCount % CHARGE_EFFECT_INTERVAL == 0 &&
		    mMoveCount >= CHARGE_EFFECT_INTERVAL)
		{
			effectManager->setEffect(&mX, &mY, CHARGE_EF_BOSS);
		}
		// 発射カウントのリセット
		mShotBulletCount = 0;
		mShotMiniBulletCount = 0;
	}
	// チャージとワープの間
	else if (mMoveCount > CHARGE_END_FRAME_BOSS &&
	         mMoveCount <= WAIT1_END_FRAME)
	{
		// チャージとワープの間、隙を晒すため何も行動しない
		// 発射カウントのリセット
		mShotBulletCount = 0;
		mShotMiniBulletCount = 0;
	}
	else if (mMoveCount > WAIT1_END_FRAME &&
	         mMoveCount <= WARP1_END_FRAME) // 281f～330fの間でワープ
	{
		// ダメージ画像を表示するため,ダメージフラグをtrue
		mIsWarp = true;
		// 弾の発射カウントのリセット
		mShotBulletCount = 0;
		mShotMiniBulletCount = 0;
		// ワープ開始。エフェクト表示
		if (mMoveCount == WARP1_TRIGGER_FRAME)
		{
			effectManager->setEffect(&mX, &mY,
			                         WARP_EF);   // ワープエフェクトの表示
			mVectorY = BOSS_ENEMY_WARP_VECTOR_Y; // y軸の速度を変更
		}
		// ワープ演出
		// 演出開始カウントを過ぎているかつ、
		// アルファ値が0以上の場合
		if (mMoveCount > WARP1_TRIGGER_FRAME && mAlpha > 0)
		{
			// 徐々に薄くなる＆徐々に浮かび上がる
			mAlpha -= FADE_ALPHA_STEP;
			mY += mVectorY;
			mVectorY *= BOSS_ENEMY_SPEEDUP_Y;
		}
	}
	// 1回目のワープ
	else if (mMoveCount == WARP1_RESET_FRAME)
	{
		// ワープ先に移動
		mX = BOSS_WARP_TOP_POINT_X;
		mY = BOSS_WARP_TOP_POINT_Y;
		// 透過度の設定
		mAlpha = ALPHA_MAX;
		mIsWarp = false;

	}
	// 330f～600fの間では攻撃中なので移動処理はなし
	// 600f～700fの間もう一度ワープの準備
	else if (mMoveCount > WARP2_START_FRAME && mMoveCount <= WARP2_END_FRAME)
	{
		// 弾発射カウントのリセット
		mShotBulletCount = 0;
		mShotMiniBulletCount = 0;
		// ワープ演出前の処理(649フレーム目で実行)
		if (mMoveCount == WARP2_ALPHA_RESET_FRAME)
		{
			// アルファ値の設定
			mAlpha = ALPHA_MAX;
		}
		// 650フレーム目で実行
		if (mMoveCount > WARP2_DAMAGE_START_FRAME)
		{
			// ダメージ画像を表示するため,ダメージフラグをtrue
			mIsWarp = true;
		}
		// ワープ開始
		// 674フレーム目で実行
		if (mMoveCount == WARP2_TRIGGER_FRAME)
		{
			effectManager->setEffect(&mX, &mY,
			                         WARP_EF);   // ワープエフェクトの表示
			mVectorY = BOSS_ENEMY_WARP_VECTOR_Y; // y軸の速度を変更
		}
		// ワープ演出
		// 演出開始カウントを過ぎているかつ、
		// アルファ値が0以上の場合
		if (mMoveCount > WARP2_TRIGGER_FRAME && mAlpha > 0)
		{
			// 徐々に薄くなる＆徐々に浮かび上がる
			mAlpha -= FADE_ALPHA_STEP;
			mY += mVectorY;
			mVectorY *= BOSS_ENEMY_SPEEDUP_Y;
		}
	}
	// 再度ワープ移動(701フレーム目で実行)
	else if (mMoveCount == WARP2_RESET_FRAME)
	{
		// ワープ先によって分岐＆ワープ先に移動
		switch (mBossDirection)
		{
		case POINT_LEFT: // 画面の左の位置
			// ワープ先に移動
			mX = BOSS_WARP_BOTTOM_POINT_X;
			mY = BOSS_WARP_BOTTOM_POINT_Y;
			mBossDirection = POINT_BOTTOM; // 次のワープ先を設定
			break;
		case POINT_BOTTOM: // 画面の下の位置
			// ワープ先に移動
			mX = BOSS_WARP_RIGHT_POINT_X;
			mY = BOSS_WARP_RIGHT_POINT_Y;
			mBossDirection = POINT_RIGHT; // 次のワープ先を設定

			break;
		case POINT_RIGHT: // 画面の右の位置
			// ワープ先に移動
			mX = BOSS_WARP_TOP_POINT_X;
			mY = BOSS_WARP_TOP_POINT_Y;
			mBossDirection = POINT_TOP; // 次のワープ先を設定

			break;
		case POINT_TOP: // 画面の上の位置
			// ワープ先に移動
			mX = BOSS_WARP_LEFT_POINT_X;
			mY = BOSS_WARP_LEFT_POINT_Y;
			mBossDirection = POINT_LEFT; // 次のワープ先を設定

			break;
		default:
			break;
		}
		// アルファ値の初期化
		mAlpha = ALPHA_MAX;
		mIsWarp = false;

	}
	// ワープ後、その位置で左右移動
	// ワープから40フレームの間行動しない
	else if (mMoveCount <= WAIT2_END_FRAME && mMoveCount > WARP2_RESET_FRAME)
	{
		// 移動開始までの間、隙を晒すため何も行動しない
		// 発射カウントのリセット
		mShotBulletCount = 0;
		mShotMiniBulletCount = 0;
	}
	// 742～822フレームの間は左へ移動
	else if (mMoveCount <= MOVE_RIGHT1_END_FRAME &&
	         mMoveCount > WAIT2_END_FRAME)
	{
		// 左移動
		mX++;
	}
	// 822～902フレームの間は右へ移動
	else if (mMoveCount <= MOVE_LEFT_END_FRAME &&
	         mMoveCount > MOVE_RIGHT1_END_FRAME)
	{
		// 右移動
		mX--;
	}
	// 822～982フレームの間は再度左へ移動
	else if (mMoveCount <= MOVE_RIGHT2_END_FRAME &&
	         mMoveCount > MOVE_LEFT_END_FRAME)
	{
		// 左移動
		mX++;
	}
	// 983フレームで移動カウントをリセット。
	else if (mMoveCount == LOOP_RESET_FRAME)
	{
		// 各カウントのリセット
		mMoveCount = 0;
		mShotBulletCount = 0;
		mShotMiniBulletCount = 0;
		// アルファ値の設定
		mAlpha = ALPHA_MAX;
	}
	// 移動カウントを更新
	mMoveCount++;
}

/*
@brief	action関数で動作させるボス敵の行動の関数
@param[in]	なし
@return		なし

@warning	ボスを倒した後の行動を作るために作成
@note	ボス敵を倒した演出だけでも十分と判断したので処理はなし
*/
void BossEnemy::moveDefeat()
{
}

/*
@brief
action関数で動作させる行動パターンを切り替える間に移動や設定を行う関数(HPが75％段階)
@param[in]	なし
@return		なし

@note	HPが75％の場合この関数へ分岐。
@note   MoveCount変数でカウントし、処理を管理
@note	positionSetCenter関数を使い次の行動の初期位置へ移動
@note
位置についたら次の行動に必要なパラーメータ設定を行い、HPを減らし次の行動へ分岐させる
@warning
- 次に分岐する行動への準備を行う
*/
void BossEnemy::settingNextPatrolMove()
{
	// 初期設定
	if (!mIsResetMoveCount)
	{
		mIsResetMoveCount = true;
		mMoveCount = 0;       // 移動カウントをリセット
		mIsUnbeatable = true; // 切り替え中はダメージを受けない
	}
	else
	{
		mMoveCount++; // 移動カウントの更新
	}
	// 移動まで少しディレイ(30フレーム)
	if (mMoveCount >= START_MOVE_DELAY_FRAME)
	{
		// 位置移動完了まで関数を実行。返り値で判定
		if (moveCenterPosition())
		{
			// 次に分岐する行動への準備
			// ダメージ受けるように、無敵フラグをfalse
			mIsUnbeatable = false;
			mIsResetMoveCount = false;
			// 次の移動の初期設定
			// 指定位置からaten2を使用して角度を設定
			mSpeed = BOSS_ENEMY_SPD;
			mMovementVectorX = BOSS_WARP_LEFT_POINT_X - mX;
			mMovementVectorY = BOSS_WARP_LEFT_POINT_Y - mY;
			mMoveTargetAngle = atan2(mMovementVectorY, mMovementVectorX);
			// 角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
			// ＨＰを減らし、次のパターンへ分岐するようにする
			mHitPoint -= START_MOVE_DAMAGE;
			mBossDirection = POINT_LEFT; // 移動の方向を画面の左の位置へ切り替え
		}
	}
}

/*
@brief
action関数で動作させる行動パターンを切り替える間に移動や設定を行う関数(HPが50％段階)
@param[in]	なし
@return		なし

@note	HPが50％の場合この関数へ分岐。
@note   MoveCount変数でカウントし、処理を管理
@note	positionSetCenter関数を使い次の行動の初期位置へ移動
@note
位置についたら次の行動に必要なパラーメータ設定を行い、HPを減らし次の行動へ分岐させる
@warning
- 次に分岐する行動への準備を行う

*/
void BossEnemy::settingNextWarpMove()
{
	mSpeed = BOSS_ENEMY_SPD;
	// 初期設定
	if (!mIsResetMoveCount)
	{
		mIsResetMoveCount = true;
		mMoveCount = 0;       // 移動カウントをリセット
		mIsUnbeatable = true; // 切り替え中はダメージを受けない
	}
	else
	{
		mMoveCount++; // 移動カウントの更新
	}
	// 移動まで少しディレイ(30フレーム)
	if (mMoveCount >= START_MOVE_DELAY_FRAME)
	{
		// 位置移動完了まで関数を実行。返り値で判定
		if (moveCenterPosition())
		{
			// 次に分岐する行動への準備
			// ダメージ受けるように、無敵フラグをfalse
			mIsUnbeatable = false;
			mIsResetMoveCount = false;
			// ＨＰを減らし、次のパターンへ分岐するようにする
			mHitPoint -= START_MOVE_DAMAGE;
			mBossDirection = POINT_TOP; // 移動の方向を画面の上の位置へ切り替え
		}
	}
}

/*
@brief
action関数で動作させる行動パターンを切り替える間に移動や設定を行う関数(HPが25％段階)
@param[in]	なし
@return		なし

@note	HPが25％の場合この関数へ分岐。
@note   MoveCount変数でカウントし、処理を管理
@note	positionSetCenter関数を使い次の行動の初期位置へ移動
@note
位置についたら次の行動に必要なパラーメータ設定を行い、HPを減らし次の行動へ分岐させる
@warning
- 次に分岐する行動への準備を行う

*/
void BossEnemy::settingNextWarpAndAttackMove()
{
	mAlpha = ALPHA_MAX;
	// 初期設定
	if (!mIsResetMoveCount)
	{
		mIsResetMoveCount = true;
		mMoveCount = 0;       // 移動カウントをリセット
		mIsUnbeatable = true; // 切り替え中はダメージを受けない
	}
	else
	{
		mMoveCount++; // 移動カウントの更新
	}
	// 移動まで少しディレイ(30フレーム)
	if (mMoveCount >= START_MOVE_DELAY_FRAME)
	{
		// 位置移動完了まで関数を実行。返り値で判定
		if (moveCenterPosition())
		{
			// 次に分岐する行動への準備
			// ダメージ受けるように、無敵フラグをfalse
			mIsUnbeatable = false;
			mIsResetMoveCount = false;
			mMoveCount = 0; // 移動カウントのリセット
			// ＨＰを減らし、次のパターンへ分岐するようにする
			mHitPoint -= START_MOVE_DAMAGE;
			mBossDirection = POINT_TOP; // 移動の方向を画面の上の位置へ切り替え
		}
	}
}

void BossEnemy::takeDamage(int *score)
{
	if (!mIsTakeDamage)
	{
		return;
	}
	// HPが０以上かつ無敵フラグ(Unbeatable)がfalseの場合はHPを減らす
	if (mHitPoint > 0)
	{
		if (!mIsUnbeatable && !mIsDamageCoolDown)
		{
			mHitPoint -= ENEMY_TAKE_DAMAGE;
			*score += HIT_SCORE; // ヒットスコアの加算
			mIsDamageCoolDown = true;
		}
		mIsTakeDamage = false;
	}
	// HPが０以下の場合は撃破フラグ(Defeat)をtrueにする
	if (mHitPoint <= 0)
	{
		mIsDefeat = true;
	}
}
/*
@brief	ボス敵の弾発射の管理を行う関数

@param[in]	BulletManager *bulletManager:弾管理クラスのポインタ
@param[in]	EffectManager *effectManager:エフェクト管理クラスのポインタ
@param[in]	ItemObjectManager *itemObjectManager:アイテム管理クラスのポインタ
@return		なし

@note

- ボスのフラグ(FLG)がtrueで撃破フラグ(Defeat)がfalseの場合のみ、処理を実行する
-
bossEnemyクラスメンバのbossShootCountmini変数及びFrameCount変数でカウントし、処理を管理している
- 発射時に発射SEの再生をしている
- 発射パターンの切り替え時には、アイテムのドロップと爆破エフェクト表示を行う

-
HPによって発射を４パターンに分岐している。段々と激しい発射パターンに変化し最後は救済アイテムがないと突破は難しい

1. HP100%～75%、
    - ミニ弾の円状の弾幕を一定間隔で発射するのを繰り返している
    - 同時に12方向へミニ弾を発射

2. HP74%～50%、　
    - ミニ弾の円状の弾幕を一定間隔で発射するのを繰り返している
    - 同時に12方向へミニ弾を発射
    - 発射開始すると4連属でミニ弾の弾幕を発射
    - 発射間隔は前の発射パターンよりも短くなっている

3. HP49%～25%、　
    - ミニ弾と通常弾の2種類の弾を発射する
        - ミニ弾の円状の弾幕を一定間隔で発射するのを繰り返している
            - 同時に12方向へミニ弾を発射
            - 発射開始すると4連属でミニ弾の弾幕を発射
            - 発射間隔は前の発射パターンよりも短くなっている
        - 通常弾の円状の弾幕を発射するのを繰り返している
            - 発射された弾は徐々に減速
            - ミニ弾の弾幕よりも発射間隔は長い
            - 30方向へ同時に通常弾を発射

4. HP24%～0%,　　
    - ミニ弾と通常弾の2種類の弾を発射する
        - ミニ弾の円状の弾幕の発射を絶え間なく繰り返している
            - 一定間隔でミニ弾の弾幕の発射角度をずらしている
            - 同時に12方向へミニ弾を発射
        - 通常弾の円状の弾幕を発射するのを繰り返している
            - 発射された弾は徐々に減速
            - ミニ弾の弾幕よりも発射間隔は長い
            - 30方向へ同時に通常弾を発射
    - 三種の雑魚敵を召喚している
    - ボスが倒された場合、召喚した敵は初期状態へ戻している

@note　2重ループのfor分には変数ｊではなくkを使用している。iとjが見分けずらいので。３重の場合はlを使用

*/
void BossEnemy::shotBossEnemyBullet(BulletManager *bulletManager,
                                    EffectManager *effectManager,
                                    ItemObjectManager *itemObjectManager,
                                    MinionEnemyManager *minionEnemyManager,
                                    Player *player)
{
	if (mIsActive && mIsDefeat == false)
	{
		mShotMiniBulletCount++; // ミニ弾の発射カウントの更新

		// HP100%～75%
		if (mBossHitPointPercent <= BOSS_HP_PER_MAX &&
		    mBossHitPointPercent > BOSS_HP_PER_PHASE1_END)
		{
			// 30Fでミニ弾を発射
			if (mShotMiniBulletCount >= BOSS_MINI_SHOT_INTERVAL_FRAME)
			{
				int miniBulletAngle;
				// 発射を12発分繰り返す。同時発射
				// 円形の弾幕
				for (int i = 0; i < BOSS_MINI_SHOT_AMOUNT; i++)
				{
					// 30毎に発射角度を変更
					miniBulletAngle = i * BOSS_MINI_SHOT_ANGLE_STEP;

					bulletManager->setBullet((int)mX, (int)mY, miniBulletAngle,
					                         ENEMY_MINI_YELLOW, false, false);
				}
				// 発射カウントのリセット
				mShotMiniBulletCount = 0;
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}
		}

		// HP75%
		else if (mBossHitPointPercent == BOSS_HP_PER_PHASE1_END)
		{
			// 一度だけ処理を実行させる
			if (mIsExecuteProcess[SECOND_EXECUTE] == FALSE)
			{
				// フラグの切り替え
				mIsExecuteProcess[SECOND_EXECUTE] = TRUE;
				// アイテムドロップ
				itemObjectManager->dropItem((int)mX, (int)mY, OBJECT_EXP,
				                            player);
				itemObjectManager->dropItem((int)mX + DROP_OFFSET_X, (int)mY,
				                            OBJECT_STAR, player);
				itemObjectManager->dropItem((int)mX, (int)mY, OBJECT_LIFE,
				                            player);
				// 行動切り替え時SEの再生
				playSoundEffect();

				// 爆破エフェクトの表示
				// 重ならないようにループ回数によって位置を変更

				effectManager->setEffect(&(mX), &(mY), EXPLOSION_EF);

				//// 爆破エフェクト表示を4個分繰り返す
				// for (int i = 0; i < BOSS_EXPLOSION_COUNT; i++)
				//{
				//	mExplosionX =
				//	    mX + BOSS_EXPLOSION_START_X + i * BOSS_EXPLOSION_STEP_X;
				//	mExplosionY = mY + BOSS_EXPLOSION_OFFSET_Y;

				//	// 爆破エフェクトの表示
				//	// 重ならないようにループ回数によって位置を変更

				//	effectManager->setEffect(&(mExplosionX), &(mExplosionY),
				//	                         EXPLOSION_EF);
				//}
				// 発射カウントのリセット
				mShotBulletCount = 0;
				mShotMiniBulletCount = 0;
			}
		}

		// HP74%～50%
		else if (mBossHitPointPercent < BOSS_HP_PER_PHASE1_END &&
		         mBossHitPointPercent > BOSS_HP_PER_PHASE2_END)
		{
			// 30フレーム間隔でミニ弾発射カウントをリセット
			if (mShotMiniBulletCount > BOSS_MINI_BURST_RESET_FRAME)
			{
				mShotMiniBulletCount = 0;
			}
			// ミニ弾発射カウントが15以内に4フレーム間隔で発射を実行(計4回
			if (mShotMiniBulletCount < BOSS_MINI_BURST_ACTIVE_FRAME &&
			    mShotMiniBulletCount % BOSS_MINI_BURST_INTERVAL_FRAME == 0)
			{
				int miniBulletAngle;
				// 発射を12発分繰り返す。同時発射
				// 円形の弾幕
				for (int i = 0; i < BOSS_MINI_SHOT_AMOUNT; i++)
				{
					// 30度毎に発射角度を変更
					miniBulletAngle = i * BOSS_MINI_SHOT_ANGLE_STEP;

					bulletManager->setBullet((int)mX, (int)mY, miniBulletAngle,
					                         ENEMY_MINI_YELLOW, false, false);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}
		}

		// HP50%
		else if (mBossHitPointPercent == BOSS_HP_PER_PHASE2_END)
		{
			// 一度だけ処理を実行させる
			if (mIsDropExperiencePoint == FALSE)
			{
				// アイテムのドロップ
				itemObjectManager->dropItem((int)mX, (int)mY, OBJECT_EXP,
				                            player);
				itemObjectManager->dropItem((int)mX + DROP_OFFSET_X, (int)mY,
				                            OBJECT_STAR, player);
				itemObjectManager->dropItem((int)mX, (int)mY, OBJECT_LIFE,
				                            player);
				// フラグの切り替え
				mIsDropExperiencePoint = TRUE;
				// 行動切り替え時SEの再生
				playSoundEffect();

				// 爆破エフェクトの表示
				// 重ならないようにループ回数によって位置を変更

				effectManager->setEffect(&(mX), &(mY), EXPLOSION_EF);
				// 発射カウントをリセット
				mShotBulletCount = 0;
				mShotMiniBulletCount = 0;
			}
		}

		// HP49%～25%
		else if (mBossHitPointPercent < BOSS_HP_PER_PHASE2_END &&
		         mBossHitPointPercent > BOSS_HP_PER_PHASE3_END)
		{
			// 30フレーム間隔でミニ弾発射カウントをリセット
			if (mShotMiniBulletCount > BOSS_MINI_BURST_RESET_FRAME)
			{
				mShotMiniBulletCount = 0;
			}

			// ミニ弾発射カウントが15以内に4フレーム間隔で発射を実行(計4回
			if (mShotMiniBulletCount < BOSS_MINI_BURST_ACTIVE_FRAME &&
			    mShotMiniBulletCount % BOSS_MINI_BURST_INTERVAL_FRAME == 0)
			{
				int miniBulletAngle;

				// 発射を12発分繰り返す。同時発射
				// ミニ弾の円形の弾幕
				for (int i = 0; i < BOSS_MINI_SHOT_AMOUNT; i++)
				{
					// 30度毎に発射角度を変更
					miniBulletAngle = i * BOSS_MINI_SHOT_ANGLE_STEP;

					bulletManager->setBullet((int)mX, (int)mY, miniBulletAngle,
					                         ENEMY_MINI_YELLOW, false, false);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}
			// 弾発射カウント更新
			mShotBulletCount++;
			// 120フレーム間隔で発射を実行
			if (mShotBulletCount >= BOSS_NOMAL_SHOT_INTERVAL_FRAME)
			{
				int ringBulletAngle;
				// 発射を30発分繰り返す。同時発射
				// 通常弾の円形の弾幕
				for (int i = 0; i < BOSS_NOMAL_SHOT_AMOUNT; i++)
				{
					// 12度毎に角度を変更
					ringBulletAngle = i * BOSS_NOMAL_SHOT_ANGLE_STEP;

					bulletManager->setBullet((int)mX, (int)mY, ringBulletAngle,
					                         ENEMY_NOMAL, true, false);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
				mShotBulletCount = 0; // 発射カウントのリセット
			}
		}

		// HP25%
		else if (mBossHitPointPercent == BOSS_HP_PER_PHASE3_END)
		{
			// 一度だけ実行させる処理
			if (mIsExecuteProcess[THIRD_EXECUTE] == FALSE)
			{
				// アイテムのドロップ
				itemObjectManager->dropItem((int)mX, (int)mY, OBJECT_EXP,
				                            player);
				itemObjectManager->dropItem((int)mX + DROP_OFFSET_X, (int)mY,
				                            OBJECT_STAR, player);
				itemObjectManager->dropItem((int)mX, (int)mY, OBJECT_LIFE,
				                            player);
				// 行動切り替え時SEの再生
				playSoundEffect();

				// 爆破エフェクトの表示
				// 重ならないようにループ回数によって位置を変更

				effectManager->setEffect(&(mX), &(mY), EXPLOSION_EF);

				// カウントのリセット、フラグの切り替え
				mIsExecuteProcess[THIRD_EXECUTE] = TRUE;
				mShotBulletCount = 0;
				mShotMiniBulletCount = 0;
			}
		}

		// HP24%～0%
		else if (mBossHitPointPercent < BOSS_HP_PER_PHASE3_END &&
		         mBossHitPointPercent > BOSS_HP_PER_DEFEAT)
		{
			// 弾発射カウントの更新
			mShotBulletCount++;
			// 30フレーム間隔でミニ弾発射カウントをリセット
			if (mShotMiniBulletCount > BOSS_MINI_BURST_RESET_FRAME)
			{
				mShotMiniBulletCount = 0;
			}
			// 15F間隔で発射角度を変更
			// 4フレーム間隔で発射
			if (mShotMiniBulletCount < BOSS_MINI_BURST_ACTIVE_FRAME &&
			    mShotMiniBulletCount % BOSS_MINI_BURST_INTERVAL_FRAME == 0)
			{
				int miniBulletAngle;
				// 発射を12発分繰り返す。同時発射
				// ミニ弾の円形の弾幕
				for (int i = 0; i < BOSS_MINI_SHOT_AMOUNT; i++)
				{
					// 30度毎に発射角度を変更
					miniBulletAngle = i * BOSS_MINI_SHOT_ANGLE_STEP;

					bulletManager->setBullet((int)mX, (int)mY, miniBulletAngle,
					                         ENEMY_MINI_YELLOW, false, false);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}
			// 15フレーム毎に発射角度をずらす
			// 4フレーム間隔で発射
			else if (mShotMiniBulletCount % BOSS_MINI_BURST_INTERVAL_FRAME == 0)
			{
				int miniBulletAngle;
				// 発射を12発分繰り返す。同時発射
				// ミニ弾の円形の弾幕
				for (int i = 0; i < BOSS_MINI_SHOT_AMOUNT; i++)
				{
					// 30度毎に発射角度を変更に加えて20度ずらす
					miniBulletAngle = i * BOSS_MINI_SHOT_ANGLE_STEP +
					                  BOSS_MINI_BURST_OFFSET_ANGLE;
					bulletManager->setBullet((int)mX, (int)mY, miniBulletAngle,
					                         ENEMY_MINI_YELLOW, false, false);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}

			// 120フレーム間隔で発射を実行
			if (mShotBulletCount >= BOSS_NOMAL_SHOT_INTERVAL_FRAME)
			{
				int ringBulletAngle;
				// 発射を30発分繰り返す。同時発射
				// 通常弾の円形の弾幕
				for (int i = 0; i < BOSS_NOMAL_SHOT_AMOUNT; i++)
				{
					// 12度毎に角度を変更
					ringBulletAngle = i * BOSS_NOMAL_SHOT_ANGLE_STEP;
					bulletManager->setBullet((int)mX, (int)mY, ringBulletAngle,
					                         ENEMY_NOMAL, true, false);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
				mShotBulletCount = 0; // 発射カウントのリセット
			}
			// 雑魚敵の召喚
			if (mIsSummonEnemy) // 召喚フラグがtrueの場合(ボス敵の移動処理で切り替え)
			{
				// 修正、マジックナンバー
				// 召喚する敵の初期化
				minionEnemyManager->mChargeEnemies[SUMMON_LEFT]->start();
				minionEnemyManager->mEnemies[SUMMON_LEFT]->start();
				minionEnemyManager->mTraceEnemies[SUMMON_LEFT]->start();

				minionEnemyManager->mChargeEnemies[SUMMON_RIGHT]->start();
				minionEnemyManager->mEnemies[SUMMON_RIGHT]->start();
				minionEnemyManager->mTraceEnemies[SUMMON_RIGHT]->start();

				// 左側召喚
				minionEnemyManager->mChargeEnemies[SUMMON_LEFT]->pop(
				    SUMMON_LEFT_CHARGE_ENEMY_X, SUMMON_ENEMY_Y, SUMMON_LEFT,
				    true);
				minionEnemyManager->mEnemies[SUMMON_LEFT]->pop(
				    SUMMON_LEFT_NOMAL_ENEMY_X, SUMMON_ENEMY_Y, SUMMON_LEFT,
				    true);
				minionEnemyManager->mTraceEnemies[SUMMON_LEFT]->pop(
				    SUMMON_LEFT_TRACE_ENEMY_X, SUMMON_ENEMY_Y, SUMMON_LEFT,
				    true);

				// 右側召喚
				minionEnemyManager->mChargeEnemies[SUMMON_RIGHT]->pop(
				    SUMMON_RIGHT_CHARGE_ENEMY_X, SUMMON_ENEMY_Y, SUMMON_RIGHT,
				    true);
				minionEnemyManager->mEnemies[SUMMON_RIGHT]->pop(
				    SUMMON_RIGHT_NOMAL_ENEMY_X, SUMMON_ENEMY_Y, SUMMON_RIGHT,
				    true);
				minionEnemyManager->mTraceEnemies[SUMMON_RIGHT]->pop(
				    SUMMON_RIGHT_TRACE_ENEMY_X, SUMMON_ENEMY_Y, SUMMON_RIGHT,
				    true);
			}
		}

		// HP0%
		else if (mBossHitPointPercent == BOSS_HP_PER_DEFEAT)
		{
			// 修正、マジックナンバー
			// 召喚した敵の初期化
			minionEnemyManager->mChargeEnemies[SUMMON_LEFT]->start();
			minionEnemyManager->mEnemies[SUMMON_LEFT]->start();
			minionEnemyManager->mTraceEnemies[SUMMON_LEFT]->start();

			minionEnemyManager->mChargeEnemies[SUMMON_RIGHT]->start();
			minionEnemyManager->mEnemies[SUMMON_RIGHT]->start();
			minionEnemyManager->mTraceEnemies[SUMMON_RIGHT]->start();
		}
	}
}
