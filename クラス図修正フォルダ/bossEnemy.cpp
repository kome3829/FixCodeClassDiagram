#include "bossEnemy.h"

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
	mBossEffect = new Effect();
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
void BossEnemy::action()
{
	// HP計算
	if (mHitPoint > 0)
	{
		//ボスの現在HPをパーセントに変換
		mBossHitPointPercent = (int)((mHitPoint / (double)BOSS_MAX_HP) * BOSS_ENEMY_MAX_HP_PERCENT);
		//HPバーの横幅を計算
		mBossHitPointBarRightEnd = BOSS_HP_BER_MAXWIDTH * mBossHitPointPercent / BOSS_ENEMY_MAX_HP_PERCENT;
	}

	//boss動き、体力応じて分岐
	//分岐の値はわかりやすいため、ラベル適宜で置き換えずそのままにしている
	if (mIsActive && mIsDefeat == false)
	{
		//マジックナンバー
		if (mBossHitPointPercent <= BOSS_HP_PER_MAX && mBossHitPointPercent > BOSS_HP_PER_PHASE1_END)				{ moveLeftAndRight();}
		else if (mBossHitPointPercent == BOSS_HP_PER_PHASE1_END)													{ settingNextPatrolMove(); }
		else if (mBossHitPointPercent < BOSS_HP_PER_PHASE1_END && mBossHitPointPercent > BOSS_HP_PER_PHASE2_END)	{ patrolMoveFourPoint(); }
		else if (mBossHitPointPercent == BOSS_HP_PER_PHASE2_END)													{ settingNextWarpMove(); }
		else if (mBossHitPointPercent < BOSS_HP_PER_PHASE2_END && mBossHitPointPercent > BOSS_HP_PER_PHASE3_END)	{ warpMoveFourPoint(); }
		else if (mBossHitPointPercent == BOSS_HP_PER_PHASE3_END)													{ settingNextWarpAndAttackMove(); }
		else if (mBossHitPointPercent < BOSS_HP_PER_PHASE3_END && mBossHitPointPercent > BOSS_HP_PER_DEFEAT)		{ warpAndAttackMove(); }
		else if(mBossHitPointPercent == BOSS_HP_PER_DEFEAT)															{ moveDefeat(); }
	}
	//撃破された場合実行
	if (mIsDefeat)
	{
		//撃破演出の透過度の更新処理
		//ゼロになるまで減算
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
- 撃破時の徐々に透明になっていく演出でこれまでとは違う強い敵を撃破したことを伝える
*/
void BossEnemy::draw()
{
	if (mIsActive)
	{
		//撃破された場合
		if (mIsDefeat)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, mAlpha);
			DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF, Data::getInstance()->mBossImageHandle, TRUE);//透過度を反映させた画像表示
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//透過度のリセット
		}
		else
		{
			//ダメージを受けた場合
			if (mIsTakeDamage)
			{
				//修正案。ダメージ時のアルファブレンドは要らない可能性があるかも
				SetDrawBlendMode(DX_BLENDMODE_ALPHA,mAlpha);
				DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF, Data::getInstance()->mBossDamageImageHandle, TRUE);//ダメージ用画像の表示
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);//透過度のリセット

				//カウントを行い、ダメージ画像の表示フレームを制限する
				mDamageDisplayCount++;
				if (mDamageDisplayCount > DAMAGE_COUNT_LIMIT)
				{
					mIsTakeDamage = false;
					mDamageDisplayCount = 0;
				}
			}
			else
			{
				//通常表示
				DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF, Data::getInstance()->mBossImageHandle, TRUE);
			}
		}
		mBossEffect->playEffectAnimation();//エフェクトの再生表示
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
	//HP = 0;
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
}

/*
@brief	メインループで動作させるボス敵のヒット判定を行う関数

@param[in]	Bullet* bullet : 弾クラスインスタンスのポインタ
@param[in]	MissileBullet* missileBullet : ミサイル弾クラスインスタンスのポインタ
@param[in]	SpecialBullet* specialBullet : スペシャル弾クラスインスタンスのポインタ
@param[in]	int* score : ゲームスコアのポインタ

@return		なし

@note	撃破フラグ(Defeat)がfalseの場合のみ処理を実行する
@note	プレイヤーの弾がボス敵にヒットしているかを判定する
@note	弾の種別によって分岐し、ボス敵と弾の位置を計算して弾が一定範囲内なら当たり判定を返す
@note	判定に応じてダメージ処理、スコア加算、撃破判定を行う
*/
void BossEnemy::checkPlayerBulletHit(Bullet* bullet, MissileBullet* missileBullet, SpecialBullet* specialBullet, int* score)
{
	if (!mIsDefeat)
	{
		//通常弾の判定
		if (mIsActive && bullet->mIsActive)
		{
			//ボス敵と弾の位置を計算して弾が一定範囲内なら当たり判定を返す
			if (abs((int)(bullet->mX - mX)) < (mWidth / CUT_HALF + HIT_RANGE_MARGIN_X) && abs((int)(bullet->mY - mY)) < mHeight / CUT_HALF + HIT_RANGE_MARGIN_Y)
			{
				bullet->mIsActive = false;//弾の無効化
				*score += HIT_SCORE;//ヒットスコアの加算
				//HPが０以上かつ無敵フラグ(Unbeatable)がfalseの場合はHPを減らす
				if (mHitPoint > 0)
				{
					if (!mIsUnbeatable)
					{
						mHitPoint -= ENEMY_TAKE_DAMAGE;
					}
					//ダメージフラグがfalseの場合、trueにする
					if (mIsTakeDamage == false)
					{
						mIsTakeDamage = true;
					}
				}	
				//HPが０以下の場合は撃破フラグ(Defeat)をtrueにする
				if (mHitPoint <= 0)
				{
					mIsDefeat = true;
				}
			}
		}
		//ミサイル弾の判定
		if (mIsActive && missileBullet->mIsActive)
		{
			//ボス敵と弾の位置を計算して弾が一定範囲内なら当たり判定を返す
			if (abs((int)(missileBullet->mX - mX)) < (mWidth / CUT_HALF + HIT_RANGE_MARGIN_X) && abs((int)(missileBullet->mY - mY)) < mHeight / CUT_HALF + HIT_RANGE_MARGIN_Y)
			{
				missileBullet->mIsActive = false;//弾の無効化
				*score += HIT_SCORE;//ヒットスコアの加算
				//HPが０以上かつ無敵フラグ(Unbeatable)がfalseの場合はHPを減らす
				if (mHitPoint > 0)
				{
					if (!mIsUnbeatable)
					{
						mHitPoint -= ENEMY_TAKE_DAMAGE;
					}
					//ダメージフラグがfalseの場合、trueにする
					if (mIsTakeDamage == false)
					{
						mIsTakeDamage = true;
					}
				}
				//HPが０以下の場合は撃破フラグ(Defeat)をtrueにする
				if (mHitPoint <= 0)
				{
					mIsDefeat = true;
				}
			}
		}
		//スペシャル弾の判定
		if (mIsActive && specialBullet->mIsActive)
		{
			//ボス敵と弾の位置を計算して弾が一定範囲内なら当たり判定を返す
			if (abs((int)(specialBullet->mX - mX)) < (mWidth / CUT_HALF + HIT_RANGE_MARGIN_X) && abs((int)(specialBullet->mY - mY)) < mHeight / CUT_HALF + HIT_RANGE_MARGIN_Y)
			{
				specialBullet->mIsActive = false;//弾の無効化
				*score += HIT_SCORE;//ヒットスコアの加算
				//HPが０以上かつ無敵フラグ(Unbeatable)がfalseの場合はHPを減らす
				if (mHitPoint > 0)
				{
					if (!mIsUnbeatable)
					{
						mHitPoint -= ENEMY_TAKE_DAMAGE;
					}
					//ダメージフラグがfalseの場合、trueにする
					if (mIsTakeDamage == false)
					{
						mIsTakeDamage = true;
					}
				}
				//HPが０以下の場合は撃破フラグ(Defeat)をtrueにする
				if (mHitPoint <= 0)
				{
					mIsDefeat = true;
				}
			}
		}
	}
}

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
	//速度が設定されていない場合のみ。設定する
	if (!mHasSetSpeed)
	{
		mSpeed = BOSS_ENEMY_SPD;
		mHasSetSpeed = true;
	}
	//逆三角関数(atan2)を使用し、移動させたい位置とボス敵の位置とのなす角を計算し、
	//角度から速度（Vx,Vy）を計算し設定している
	mCenterPositionVectorX = SCREEN_CENTER_X - mX;
	mCenterPositionVectorY = BOSS_CENTER__POSITION_Y - mY;
	mCentralPositionAngle = atan2(mCenterPositionVectorY,mCenterPositionVectorX);
	mVectorX = cos(mCentralPositionAngle) * mSpeed;
	mVectorY = sin(mCentralPositionAngle) * mSpeed;
	//指定位置と等しくなるまで位置に速度を足して移動する
	if (mX != SCREEN_CENTER_X)
	{
		mX += mVectorX;
	}
	if (mY != BOSS_CENTER__POSITION_Y)
	{
		mY += mVectorY;
	}
	//ボス敵と画面中央上の座標ずれが許容範囲内になるまで移動を実行する
	if (abs(SCREEN_CENTER_X - (int)mX) < POSITION_TOLERANCE && abs(BOSS_CENTER__POSITION_Y - (int)mY) < POSITION_TOLERANCE)
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
	//降下移動　y＝40の位置まで
	//修正案。なぜ画像サイズの半分足しているのか必要があるのか調べる
	if (mY <= mHeight / CUT_HALF + BOSS_DESCEND_LIMIT_Y)
	{
		mVectorY *= BOSS_ENEMY_SLOWDOWN_Y;//徐々に減速
		mY += mVectorY;
	}
	//左右往復移動
	else
	{
		switch (mBossDirection)//進行方向によって分岐
		{

		case LEFT://左移動
			mX -= mVectorX;
			//左端まで移動した場合
			if (mX <=  mWidth / CUT_HALF)
			{
				mBossDirection = RIGHT;//移動方向を右に切り替え
			}
			break;

		case RIGHT://右移動
			mX += mVectorX;
			//右端まで移動した場合
			if (mX >= MAX_SCREEN_WIDTH - mWidth / CUT_HALF)
			{
				mBossDirection = LEFT;//移動方向を左に切り替え
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
	mMoveCount++;//移動カウントの更新
	//位置に速度を加算して移動
	mX += mVectorX;
	mY += mVectorY;
	//ボス敵の向かう方向に応じて処理を分岐
	switch (mBossDirection)
	{
	case POINT_LEFT://左側の位置
		//移動カウントが15フレームごとに処理を実行
		if (mMoveCount % BOSS_MOVE_INTERVAL_FRAME == 0)
		{
			//一定値まで徐々にスピードダウン
			if (mSpeed > BOSS_ENEMY_SPD_DOWN_LIMIT)
			{
				mSpeed *= BOSS_ENEMY_SLOWDOWN;
			}
			//角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
		}
		//ボス敵と移動させたい位置の座標ずれが許容範囲内になるまで実行する
		//修正案。y座標の条件式< POSITION_TOLERANCEが抜けていると思われる
		if (abs(BOSS_WARP_LEFT_POINT_X - (int)mX) < POSITION_TOLERANCE && abs(BOSS_WARP_LEFT_POINT_Y - (int)mY))
		{
			//atan2を使用して次に向かう位置から角度を計算
			mMovementVectorX = BOSS_WARP_BOTTOM_POINT_X - mX;
			mMovementVectorY = BOSS_WARP_BOTTOM_POINT_Y - mY;
			mMoveTargetAngle = atan2(mMovementVectorY, mMovementVectorX);
			//角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
			mSpeed = BOSS_ENEMY_SPD;//初速を設定
			mBossDirection = POINT_BOTTOM;//向かう方向を画面の下の位置に切り替え
		}
		break;

	case POINT_BOTTOM://下の位置
		//移動カウントが15フレームごとに処理を実行
		if (mMoveCount % BOSS_MOVE_INTERVAL_FRAME == 0)
		{
			//一定値まで徐々にスピードダウン
			if (mSpeed > BOSS_ENEMY_SPD_DOWN_LIMIT)
			{
				mSpeed *= BOSS_ENEMY_SLOWDOWN;
			}
			//角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
		}
		//ボス敵と移動させたい位置の座標ずれが許容範囲内になるまで実行する
		//修正案。y座標の条件式< POSITION_TOLERANCEが抜けていると思われる
		if (abs(BOSS_WARP_BOTTOM_POINT_X - (int)mX) < POSITION_TOLERANCE && abs(BOSS_WARP_BOTTOM_POINT_Y - (int)mY))
		{
			//atan2を使用して次に向かう位置から角度を計算
			mMovementVectorX = BOSS_WARP_RIGHT_POINT_X - mX;
			mMovementVectorY = BOSS_WARP_RIGHT_POINT_Y - mY;
			mMoveTargetAngle = atan2(mMovementVectorY, mMovementVectorX);
			//角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
			mBossDirection = POINT_RIGHT;//向かう方向を画面の右の位置に切り替え
			mSpeed = BOSS_ENEMY_SPD;//初速を設定
		}
		break;

	case POINT_RIGHT://右の位置
		//一定フレーム毎で実行
		if (mMoveCount % BOSS_MOVE_INTERVAL_FRAME == 0)
		{
			//一定値まで徐々にスピードダウン
			if (mSpeed > BOSS_ENEMY_SPD_DOWN_LIMIT)
			{
				mSpeed *= BOSS_ENEMY_SLOWDOWN;
			}
			//角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
		}
		//ボス敵と移動させたい位置の座標ずれが許容範囲内になるまで実行する
		//修正案。y座標の条件式< POSITION_TOLERANCEが抜けていると思われる
		if (abs(BOSS_WARP_RIGHT_POINT_X - (int)mX) < POSITION_TOLERANCE && abs(BOSS_WARP_RIGHT_POINT_Y - (int)mY))
		{
			//atan2を使用して次に向かう位置から角度を計算
			mMovementVectorX = BOSS_WARP_TOP_POINT_X - mX;
			mMovementVectorY = BOSS_WARP_TOP_POINT_Y - mY;
			mMoveTargetAngle = atan2(mMovementVectorY, mMovementVectorX);
			//角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
			mBossDirection = POINT_TOP;//向かう方向を画面の上の位置に切り替え
			mSpeed = BOSS_ENEMY_SPD;//初速を設定
		}
		break;

	case POINT_TOP://上の位置
		//一定フレーム毎で実行
		if (mMoveCount % BOSS_MOVE_INTERVAL_FRAME == 0)
		{
			//一定値まで徐々にスピードダウン
			if (mSpeed > BOSS_ENEMY_SPD_DOWN_LIMIT)
			{
				mSpeed *= BOSS_ENEMY_SLOWDOWN;
			}
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
		}
		//ボス敵と移動させたい位置の座標ずれが許容範囲内になるまで実行する
		//修正案。y座標の条件式< POSITION_TOLERANCEが抜けていると思われる
		if (abs(BOSS_WARP_TOP_POINT_X - (int)mX) < POSITION_TOLERANCE && abs(BOSS_WARP_TOP_POINT_Y - (int)mY))
		{
		//次に向かう方向、と角度、初速を設定
			mMovementVectorX = BOSS_WARP_LEFT_POINT_X - mX;
			mMovementVectorY = BOSS_WARP_LEFT_POINT_Y - mY;
			mMoveTargetAngle = atan2(mMovementVectorY, mMovementVectorX);
			//角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
			mBossDirection = POINT_LEFT;//向かう方向を画面の左位置に切り替え
			mSpeed = BOSS_ENEMY_SPD;//初速を設定
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
void BossEnemy::warpMoveFourPoint()
{
	mMoveCount++;//移動カウントの更新
	//0～75フレーム間は左へ移動
	if (mMoveCount <= MOVE_PHASE1_END&& mMoveCount >= 0)
	{
		//左移動
		mX--;
		if (mMoveCount <= SHOT_STOP_INTERVAL)//弾発射を40フレームだけ止める
		{
			mShotBulletCount = 0;
			mShotMiniBulletCount = 0;
		}
	}
	//75～225フレーム間は右へ移動
	else if (mMoveCount <= MOVE_PHASE2_END && mMoveCount > MOVE_PHASE1_END)
	{
		//右移動
		mX++;
	}
	//225～300フレーム間はもう一度左へ移動
	else if (mMoveCount <= MOVE_PHASE3_END && mMoveCount > MOVE_PHASE2_END)
	{
		//再度左移動
		mX--;
	}
	//300～360フレーム間はワープまでの待機時間
	//ワープ前の事前に必要な処理を行う
	else if (mMoveCount <= MOVE_PHASE4_END && mMoveCount > MOVE_PHASE3_END)
	{
		//ダメージ画像を表示するため,ダメージフラグをtrue
		mIsTakeDamage = true;
		//弾の発射カウントのリセット
		mShotBulletCount = 0;
		mShotMiniBulletCount = 0;
		//ワープ演出開始
		if (mMoveCount == WARP_TRIGGER_FRAME_BOSS)
		{
			mBossEffect->mAlpha = ALPHA_MAX;//透過度の設定
			mBossEffect->setEffect(&mX, &mY, WARP_EF);//ワープエフェクトを表示
			mVectorY = BOSS_ENEMY_WARP_VECTOR_Y;//y軸の速度を変更
		}
		//ワープ演出
		//演出開始カウントを過ぎているかつ、
		//アルファ値が0以上の場合
		if (mMoveCount > WARP_TRIGGER_FRAME_BOSS &&mAlpha>0)
		{
			//徐々に薄くなる＆徐々に浮かび上がる
			mAlpha -= FADE_ALPHA_STEP;
			mY += mVectorY;
			mVectorY *= BOSS_ENEMY_SPEEDUP_Y;
		}
	}
	//ワープ実行フレーム(361)
	else if (mMoveCount == MOVE_END_FRAME )
	{
		//ワープ先によって分岐
		switch (mBossDirection)
		{
		case POINT_LEFT://画面の左の位置
			//ワープ先に移動
			mX = BOSS_WARP_BOTTOM_POINT_X;
			mY = BOSS_WARP_BOTTOM_POINT_Y;
			mBossDirection = POINT_BOTTOM;//次のワープ先を設定
			break;
		case POINT_BOTTOM://画面の下の位置
			//ワープ先に移動
			mX = BOSS_WARP_RIGHT_POINT_X;
			mY = BOSS_WARP_RIGHT_POINT_Y;
			mBossDirection = POINT_RIGHT;//次のワープ先を設定

			break;
		case POINT_RIGHT://画面の右の位置
			//ワープ先に移動
			mX = BOSS_WARP_TOP_POINT_X;
			mY = BOSS_WARP_TOP_POINT_Y;
			mBossDirection = POINT_TOP;//次のワープ先を設定

			break;
		case POINT_TOP://画面の上の位置
			//ワープ先に移動
			mX = BOSS_WARP_LEFT_POINT_X;
			mY = BOSS_WARP_LEFT_POINT_Y;
			mBossDirection = POINT_LEFT;//次のワープ先を設定

			break;
		default:
			break;
		}
		//各カウント変数とアルファ値の初期化
		mShotBulletCount = 0;
		mShotMiniBulletCount = 0;
		mMoveCount = 0;
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
		2. 溜め終わると 画面中央上のポイントへワープ移動して動かずに激しい攻撃を行う

	- ワープで巡回する移動
		- 指定カウントで次のポイントへワープ
		- ワープ移動の合間はそのポイント座標で左右に往復する移動する
		- ボス敵の向かう方向に応じて移動処理を分岐している
@warning
- これまでの移動パターンを組み合わせて、より難易度を難しくする
- 行動パターンを2つ用意することで更に難しくする
*/
void BossEnemy::warpAndAttackMove()
{
    if (mMoveCount <= CHARGE_END_FRAME_BOSS)//240fまでチャージフェーズ
    {
		//雑魚敵召喚フラグを指定フレームだけでtrue
		if (mMoveCount == SUMMON_ENEMY_SET_FRAME)
		{
			mIsSummonEnemy = true;
		}
        else
        {
            mIsSummonEnemy = false;
        }
		//エフェクトを表示。チャージフェーズ間、再生が終わるごとに再度表示
		if (mMoveCount % CHARGE_EFFECT_INTERVAL == 0 && mMoveCount >= CHARGE_EFFECT_INTERVAL)
		{
			mBossEffect->setEffect(&mX, &mY, CHARGE_EF_BOSS);//チャージエフェクトの表示
		}
		//発射カウントのリセット
        mShotBulletCount = 0;
        mShotMiniBulletCount = 0;
    }
	//チャージとワープの間
    else if (mMoveCount > CHARGE_END_FRAME_BOSS && mMoveCount <= WAIT1_END_FRAME)
    {
		//チャージとワープの間、隙を晒すため何も行動しない
		//発射カウントのリセット
        mShotBulletCount = 0;
        mShotMiniBulletCount = 0;
    }
    else if (mMoveCount > WAIT1_END_FRAME && mMoveCount <= WARP1_END_FRAME)//281f～330fの間でワープ
	{
		//ダメージ画像を表示するため,ダメージフラグをtrue
        mIsTakeDamage = true;
		//弾の発射カウントのリセット
        mShotBulletCount = 0;
        mShotMiniBulletCount = 0;
		//ワープ開始。エフェクト表示
        if (mMoveCount == WARP1_TRIGGER_FRAME)
        {
            mBossEffect->mAlpha = ALPHA_MAX;//透過度の設定
            mBossEffect->setEffect(&mX, &mY, WARP_EF);//ワープエフェクトの表示
            mVectorY = BOSS_ENEMY_WARP_VECTOR_Y;//y軸の速度を変更
        }
		//ワープ演出
		//演出開始カウントを過ぎているかつ、
		//アルファ値が0以上の場合
        if (mMoveCount > WARP1_TRIGGER_FRAME && mAlpha > 0)
        {
			//徐々に薄くなる＆徐々に浮かび上がる
            mAlpha -= FADE_ALPHA_STEP;
            mY += mVectorY;
            mVectorY *= BOSS_ENEMY_SPEEDUP_Y;
        }
    }
	//1回目のワープ
    else if (mMoveCount == WARP1_RESET_FRAME)
    {
		//ワープ先に移動
        mX = BOSS_WARP_TOP_POINT_X;
        mY = BOSS_WARP_TOP_POINT_Y;
		//透過度の設定
        mAlpha = ALPHA_MAX;
    }
	//330f～600fの間では攻撃中なので移動処理はなし
	//600f～700fの間もう一度ワープの準備
    else if (mMoveCount > WARP2_START_FRAME && mMoveCount <= WARP2_END_FRAME)
    {
		//弾発射カウントのリセット
        mShotBulletCount = 0;
        mShotMiniBulletCount = 0;
		//ワープ演出前の処理(649フレーム目で実行)
        if (mMoveCount == WARP2_ALPHA_RESET_FRAME)
        {
			//アルファ値の設定
            mAlpha = ALPHA_MAX;
        }
		//650フレーム目で実行
        if (mMoveCount > WARP2_DAMAGE_START_FRAME)
        {
			//ダメージ画像を表示するため,ダメージフラグをtrue
            mIsTakeDamage = true;
        }
		//ワープ開始
		//674フレーム目で実行
        if (mMoveCount == WARP2_TRIGGER_FRAME)
        {
            mBossEffect->mAlpha = ALPHA_MAX;//エフェクトのアルファ値の設定
            mBossEffect->setEffect(&mX, &mY, WARP_EF);//ワープエフェクトの表示
            mVectorY = BOSS_ENEMY_WARP_VECTOR_Y;//y軸の速度を変更
        }
		//ワープ演出
		//演出開始カウントを過ぎているかつ、
		//アルファ値が0以上の場合
        if (mMoveCount > WARP2_TRIGGER_FRAME && mAlpha > 0)
        {
			//徐々に薄くなる＆徐々に浮かび上がる
            mAlpha -= FADE_ALPHA_STEP;
            mY += mVectorY;
            mVectorY *= BOSS_ENEMY_SPEEDUP_Y;
        }
    }
	//再度ワープ移動(701フレーム目で実行)
    else if (mMoveCount == WARP2_RESET_FRAME)
    {
		//ワープ先によって分岐＆ワープ先に移動
		switch (mBossDirection)
		{
		case POINT_LEFT://画面の左の位置
			//ワープ先に移動
			mX = BOSS_WARP_BOTTOM_POINT_X;
			mY = BOSS_WARP_BOTTOM_POINT_Y;
			mBossDirection = POINT_BOTTOM;//次のワープ先を設定
			break;
		case POINT_BOTTOM://画面の下の位置
			//ワープ先に移動
			mX = BOSS_WARP_RIGHT_POINT_X;
			mY = BOSS_WARP_RIGHT_POINT_Y;
			mBossDirection = POINT_RIGHT;//次のワープ先を設定

			break;
		case POINT_RIGHT://画面の右の位置
			//ワープ先に移動
			mX = BOSS_WARP_TOP_POINT_X;
			mY = BOSS_WARP_TOP_POINT_Y;
			mBossDirection = POINT_TOP;//次のワープ先を設定

			break;
		case POINT_TOP://画面の上の位置
			//ワープ先に移動
			mX = BOSS_WARP_LEFT_POINT_X;
			mY = BOSS_WARP_LEFT_POINT_Y;
			mBossDirection = POINT_LEFT;//次のワープ先を設定

			break;
		default:
			break;
		}
		//アルファ値の初期化
        mAlpha = ALPHA_MAX;
    }
	//ワープ後、その位置で左右移動
	//ワープから40フレームの間行動しない
    else if (mMoveCount <= WAIT2_END_FRAME && mMoveCount > WARP2_RESET_FRAME )
    {
		//移動開始までの間、隙を晒すため何も行動しない
		//発射カウントのリセット
        mShotBulletCount = 0;
        mShotMiniBulletCount = 0;
    }
	//742～822フレームの間は左へ移動
    else if (mMoveCount <= MOVE_RIGHT1_END_FRAME && mMoveCount > WAIT2_END_FRAME)
    {
		//左移動
        mX++;
    }
	//822～902フレームの間は右へ移動
    else if (mMoveCount <= MOVE_LEFT_END_FRAME && mMoveCount > MOVE_RIGHT1_END_FRAME)
    {
		//右移動
        mX--;
    }
	//822～982フレームの間は再度左へ移動
    else if (mMoveCount <= MOVE_RIGHT2_END_FRAME && mMoveCount > MOVE_LEFT_END_FRAME)
    {
		//左移動
        mX++;
    }
	//983フレームで移動カウントをリセット。
    else if (mMoveCount == LOOP_RESET_FRAME)
    {
		//各カウントのリセット
        mMoveCount = 0;
        mShotBulletCount = 0;
        mShotMiniBulletCount = 0;
		//アルファ値の設定
        mAlpha = ALPHA_MAX;
    }
	//移動カウントを更新
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
@brief	action関数で動作させる行動パターンを切り替える間に移動や設定を行う関数(HPが75％段階)
@param[in]	なし
@return		なし

@note	HPが75％の場合この関数へ分岐。
@note   MoveCount変数でカウントし、処理を管理
@note	positionSetCenter関数を使い次の行動の初期位置へ移動
@note	位置についたら次の行動に必要なパラーメータ設定を行い、HPを減らし次の行動へ分岐させる
@warning
- 次に分岐する行動への準備を行う
*/
void BossEnemy::settingNextPatrolMove()
{
	//初期設定
	if (!mIsResetMoveCount)
	{
		mIsResetMoveCount = true;
		mMoveCount = 0;//移動カウントをリセット
		mIsUnbeatable = true;//切り替え中はダメージを受けない
	}
	else
	{
		mMoveCount++;//移動カウントの更新
	}
	//移動まで少しディレイ(30フレーム)
	if (mMoveCount >= START_MOVE_DELAY_FRAME)
	{
		//位置移動完了まで関数を実行。返り値で判定
		if (moveCenterPosition())
		{
			//次に分岐する行動への準備
			//ダメージ受けるように、無敵フラグをfalse
			mIsUnbeatable = false;
			mIsResetMoveCount = false;
			//次の移動の初期設定
			//指定位置からaten2を使用して角度を設定
			mSpeed = BOSS_ENEMY_SPD;
			mMovementVectorX= BOSS_WARP_LEFT_POINT_X - mX;
			mMovementVectorY= BOSS_WARP_LEFT_POINT_Y - mY;
			mMoveTargetAngle = atan2(mMovementVectorY, mMovementVectorX);
			//角度から速度を計算
			mVectorX = cos(mMoveTargetAngle) * mSpeed;
			mVectorY = sin(mMoveTargetAngle) * mSpeed;
			//ＨＰを減らし、次のパターンへ分岐するようにする
			mHitPoint -= START_MOVE_DAMAGE;
			mBossDirection = POINT_LEFT;//移動の方向を画面の左の位置へ切り替え
		}
	}
}

/*
@brief	action関数で動作させる行動パターンを切り替える間に移動や設定を行う関数(HPが50％段階)
@param[in]	なし
@return		なし

@note	HPが50％の場合この関数へ分岐。
@note   MoveCount変数でカウントし、処理を管理
@note	positionSetCenter関数を使い次の行動の初期位置へ移動
@note	位置についたら次の行動に必要なパラーメータ設定を行い、HPを減らし次の行動へ分岐させる
@warning
- 次に分岐する行動への準備を行う

*/
void BossEnemy::settingNextWarpMove()
{
	mSpeed = BOSS_ENEMY_SPD;
	//初期設定
	if (!mIsResetMoveCount)
	{
		mIsResetMoveCount = true;
		mMoveCount = 0;//移動カウントをリセット
		mIsUnbeatable = true;//切り替え中はダメージを受けない
	}
	else
	{
		mMoveCount++;//移動カウントの更新
	}
	//移動まで少しディレイ(30フレーム)
	if (mMoveCount >= START_MOVE_DELAY_FRAME)
	{
		//位置移動完了まで関数を実行。返り値で判定
		if (moveCenterPosition())
		{
			//次に分岐する行動への準備
			//ダメージ受けるように、無敵フラグをfalse
			mIsUnbeatable = false;
			mIsResetMoveCount = false;
			//ＨＰを減らし、次のパターンへ分岐するようにする
			mHitPoint -= START_MOVE_DAMAGE;
			mBossDirection = POINT_TOP;//移動の方向を画面の上の位置へ切り替え
		}
	}
}

/*
@brief	action関数で動作させる行動パターンを切り替える間に移動や設定を行う関数(HPが25％段階)
@param[in]	なし
@return		なし

@note	HPが25％の場合この関数へ分岐。
@note   MoveCount変数でカウントし、処理を管理
@note	positionSetCenter関数を使い次の行動の初期位置へ移動
@note	位置についたら次の行動に必要なパラーメータ設定を行い、HPを減らし次の行動へ分岐させる
@warning
- 次に分岐する行動への準備を行う

*/
void BossEnemy::settingNextWarpAndAttackMove()
{
	mAlpha = ALPHA_MAX;
	//初期設定
	if (!mIsResetMoveCount)
	{
		mIsResetMoveCount = true;
		mMoveCount = 0;//移動カウントをリセット
		mIsUnbeatable = true;//切り替え中はダメージを受けない
	}
	else
	{
		mMoveCount++;//移動カウントの更新
	}
	//移動まで少しディレイ(30フレーム)
	if (mMoveCount >= START_MOVE_DELAY_FRAME)
	{
		//位置移動完了まで関数を実行。返り値で判定
		if (moveCenterPosition())
		{
			//次に分岐する行動への準備
			//ダメージ受けるように、無敵フラグをfalse
			mIsUnbeatable = false;
			mIsResetMoveCount = false;
			mMoveCount = 0;//移動カウントのリセット
			//ＨＰを減らし、次のパターンへ分岐するようにする
			mHitPoint -= START_MOVE_DAMAGE;
			mBossDirection = POINT_TOP;//移動の方向を画面の上の位置へ切り替え
		}
	}
}
