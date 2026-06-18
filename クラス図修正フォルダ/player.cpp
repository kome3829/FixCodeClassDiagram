#include "player.h"
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
	 mWidth = PLAYER_WIDTH;
	 mHeight = PLAYER_HEIGHT;
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
	- 左右上下キーの内、押されたキーに応じて、位置（ｘ,ｙ）を移動速度(spd)で加算、減算している
	- シフトキーを同時に押すことで、移動速度(spd)を遅くする事できるようにしている

- 無敵時間の更新処理を行う
	- 無敵時間のカウント(UnbeatableCount)と無敵フラグ（Unbeatable）でタイミングや動作を制御している
	- 無敵フラグに応じて、無敵時間表示ゲージ用の変数の計算処理を行う
-ダメージ時の更新処理を行う
@warning
- 移動処理の分岐はせずに同時押し可能にすることで斜め移動も可能にしている
- 遅い移動で細かい移動操作を行えるようにしている
*/
void Player::action()
{
	//player操作
	//左移動
	if (checkPushkey(KEY_INPUT_LEFT))
	{
		mX -= mSpeed;
		//画面外へ出ないように制限
		if (mX <= mWidth / CUT_HALF)
		{
			mX = mWidth / CUT_HALF;
		}
	}

	//右移動
	if (checkPushkey(KEY_INPUT_RIGHT))
	{
		mX += mSpeed;
		//画面外へ出ないように制限
		if (mX >= MAX_SCREEN_WIDTH - mWidth / CUT_HALF)
		{
			mX = MAX_SCREEN_WIDTH - mWidth / CUT_HALF;
		}
	}

	//上移動
	if (checkPushkey(KEY_INPUT_UP))
	{
		mY -= mSpeed;
		//画面外へ出ないように制限
		//修正案。条件式に/2の追加。
		if (mY <= mHeight)
		{
			mY =  mHeight;
		}
	}

	//下移動
	if (checkPushkey(KEY_INPUT_DOWN))
	{
		mY += mSpeed;
		//画面外へ出ないように制限
		if (mY >= MAX_SCREEN_HEIGHT - mHeight)
		{
			mY = MAX_SCREEN_HEIGHT - mHeight;
		}
	}

	//左シフトで速度ダウン
	if (checkPushkey(KEY_INPUT_LSHIFT))
	{
		mSpeed = PLAYER_LOW_SPEED;
	}
	else
	{
		mSpeed = PLAYER_SPEED;
	}
	//ダメージフラグがtrueの場合
	if (mIsTakeDamage)
	{
		mDamageDisplayCount++;//ダメージカウントの更新
		if (mDamageDisplayCount == PLAYER_DAMAGE_RESET_FRAME)//ダメージカウントが40フレームでリセット
		{
			mIsTakeDamage = false;
			mDamageDisplayCount = 0;
		}
	}

	//無敵かつ無敵時間内なら
	if (mIsUnbeatable && mUnbeatableCount <= PLAYER_UNBEATABLE_TIME)
	{
		mUnbeatableCount++;//無敵時間カウントの更新
	}
	//無敵じゃない時間外、どちらかを満たしていれば
	else
	{
		//無敵フラグ、無敵時間カウントのリセット
		mIsUnbeatable = false;
		mUnbeatableCount = 0;
	}
	//無敵フラグがtrueの場合
	if (mIsUnbeatable)
	{
		//無敵時間ゲージの表示割合の更新
		mUnbeatableGagePercent =(PLAYER_UNBEATABLE_MAX_FRAME - (double)mUnbeatableCount) /PLAYER_UNBEATABLE_MAX_FRAME;
	}
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note      

- ダメージフラグ（damageFlg）がtrueの場合はダメージ用の画像を表示する
- 無敵フラグ(Unbeatable)がtrueの場合は無敵用の画像を表示する
	- 残り無敵時間のタイマーゲージを表示する
- なにもなければ通常の画像を表示する
- プレイヤーの残り残機を表示する

*/
void Player::draw()
{
	//playerの表示
	if (mIsTakeDamage)//ダメージ用画像の表示
	{
		DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF, Data::getInstance()->mPleyerDamageImageHandle, true);
	}
	else if (mIsUnbeatable)//無敵用画像の表示
	{
		//残り時間が一定以下(60フレーム)になると、無敵時の画像と通常時の画像を交互に表示
		if (mUnbeatableCount < UNBEATABLE_FLASH_START_FRAME ||(mUnbeatableCount / PLAYER_UNBEATABLE_FLASH_INTERVAL_FRAME) % EVEN_NUMBER == 0)
		{
			//無敵用画像の表示
			DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF, Data::getInstance()->mPleyerUnbeatableImageHandle, true);
		}
		else
		{
			//通常画像の表示
			DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF, Data::getInstance()->mPleyerImageHandle, true);
		}
		//無敵時間表示ゲージ
		//プレイヤーの画像の下に表示のための位置の計算
		int unbeatableGageLeftX = (int)mX - PLAYER_UNBEATABLE_GAUGE_LEFT_OFFSET;//左端
		int unbeatableGageTopY = (int)mY + PLAYER_UNBEATABLE_GAUGE_TOP_OFFSET;//上端
		int unbeatableGageRightX = (int)mX - PLAYER_UNBEATABLE_GAUGE_LEFT_OFFSET + (int)(mUnbeatableGagePercent * PLAYER_UNBEATABLE_GAUGE_WIDTH);//右端
		int unbeatableGageBottomY = (int)mY + PLAYER_UNBEATABLE_GAUGE_TOP_OFFSET + PLAYER_UNBEATABLE_GAUGE_HEIGHT;//下橋
		//ゲージの表示
		DrawBox(unbeatableGageLeftX, unbeatableGageTopY, unbeatableGageRightX, unbeatableGageBottomY,ORANGE_COLLAR,TRUE);
	}
	else//通常画像を表示
	{
		DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF, Data::getInstance()->mPleyerImageHandle, true);
	}

	//HPをアイコンで表示
	for (int i = 0; i < PLAYER_LIFE_ICON_AMOUNT; i++)
	{
		if (mIsActiveLifeIcon[i])//残りHPに応じて表示アイコンを管理
		{
			//HPアイコンの表示
			//プレイヤーの下の位置に表示
			//ループ回数によって位置をずらす
			DrawGraph((int)mX + PLAYER_LIFE_ICON_BASE_X_OFFSET -PLAYER_LIFE_ICON_SPACING_X * (PLAYER_LIFE_ICON_INDEX_MAX - i),(int)mY + PLAYER_LIFE_ICON_Y_OFFSET,Data::getInstance()->mLifeItemImageHandle,TRUE);
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

	for (int i = 0; i < MAX_HP; i++)
	{
		mIsActiveLifeIcon[i] = TRUE;
	}
}

/*
@brief	プレイヤーが敵の弾に当たるか判定を行う関数

@param[in]	EnemyBullet*     enemyBullet			:敵弾クラスのインスタンスポインタ
@param[in]	EnemyMiniBullet* enemyMiniBullet			:敵ミニ弾クラスのインスタンスポインタ

@return		ヒット判定:bool:当たった　true/ 当たってない　false

@note      

- 判定の成否を示すため、返り値はbool型としている
- ダメージフラグ（damageFlg）と撃破フラグ(dethFlg)がfalseの場合のみ処理を実行する
- 2種類の敵の弾を判定している
- 各敵の弾のフラグ(flg)がtrueの場合のみ判定処理を実行する
- プレイヤーに一定距離まで近づいたら当たったと判定している
	-判定に応じて、ダメージ処理と残り残機の判定も行う

*/
bool Player::checkEnemyBulletHit(EnemyBullet* enemyBullet, EnemyMiniBullet* enemyMiniBullet)
{
	if (!mIsTakeDamage && !mIsDefeat)
	{
		if (enemyBullet->mIsActive)//通常敵弾の判定
		{
			//プレイヤーに一定距離まで近づいたら当たったと判定
			if (abs((int)(enemyBullet->mX - mX)) < PLAYER_HIT_RANGE &&abs((int)(enemyBullet->mY - mY)) < PLAYER_HIT_RANGE)
			{
				enemyBullet->mIsActive = false;//弾の無効化
				if (!mIsUnbeatable)
				{
					//ダメージ処理
					mLife--;//HPを減らす
					mIsActiveLifeIcon[mLife] = false;//表示アイコン用のフラグをfalse
					mIsTakeDamage = true;//ダメージフラグをtrue
					PlaySoundMem(Data::getInstance()->mPlayerDamageSoundEffectHandle, DX_PLAYTYPE_BACK, TRUE);//ダメージSEの再生
				}

				if (mLife <= 0)//撃破判定
				{
					mIsDefeat = true;//撃破フラグのtrue
					return true;
				}
				return true;
			}
		}

		if (enemyMiniBullet->mIsActive)//ミニ敵弾の判定
		{
			//プレイヤーに一定距離まで近づいたら当たったと判定
			if (abs((int)(enemyMiniBullet->mX - mX)) < PLAYER_HIT_RANGE &&abs((int)(enemyMiniBullet->mY - mY)) < PLAYER_HIT_RANGE)
			{
				enemyMiniBullet->mIsActive = false;//弾の無効化
				if (!mIsUnbeatable)
				{
					//ダメージ処理
					mLife--;//HPを減らす
					mIsActiveLifeIcon[mLife] = false;//表示アイコン用のフラグをfalse
					mIsTakeDamage = true;//ダメージフラグをtrue
					PlaySoundMem(Data::getInstance()->mPlayerDamageSoundEffectHandle, DX_PLAYTYPE_BACK, TRUE);//ダメージSEの再生
				}
				if (mLife <= 0)//撃破判定
				{
					mIsDefeat = true;//撃破フラグのtrue
					return true;
				}
				return true;
			}
		}
	}
	return false;
}

/*
@brief	各アイテムオブジェクトがプレイヤーへ当たったか判定を行う関数

@param[in]	Object* itemObject			:オブジェクトクラスのインスタンスポインタ
@param[in]	int*    score		:ゲームスコアのポインタ

@return		当たったアイテムの種類:int
@note  経験値アイテム　OBJECT_EXP  1
@note  回復アイテム　OBJECT_LIFE 2
@note  無敵アイテム　OBJECT_STAR 3
@note      

- 当たったアイテムオブジェクトを示すため、返り値をint型としている
- アイテムオブジェクトのフラグ(flg)がtrueの場合のみ処理を実行する
- プレイヤーに一定距離まで近づいたら当たったと判定する
- 当たった場合、アイテムオブジェクトに応じて処理を分岐する

	1. 経験値アイテム
		- ショットパワーを増やす

	2. 回復アイテム
		- 残機を1つ増やす
		- 残機が最大の場合は増やさない
		
	3. 無敵アイテム
		- 無敵にする
		- 無敵の場合は残り時間を最大値まで戻す

*/
int Player::checkItemObjectHit(Object* itemObject ,int* score)
{
	if (itemObject->mIsActive && abs((int)(itemObject->mX - mX)) < ITEM_OBJECT_WIDTH && abs((int)(itemObject->mY - mY)) < ITEM_OBJECT_HEIGHT)
	{
		switch (itemObject->mItemObjectType)
		{
		case OBJECT_EXP://経験値アイテム
			PlaySoundMem(Data::getInstance()->mEXPItemGetSoundEffectHandle, DX_PLAYTYPE_BACK, TRUE);//アイテム獲得SEの再生
			(score) += EXP_SCORE;//経験値スコアの加算
			mShotPower += SHOT_POWER_INCREASE_AMONT;//ショットパワーが加算
			//修正案。初期化関数を作成して、start関数での初期化を変更する
			itemObject->start();//ヒットしたアイテムオブジェクトの初期化

			break;

		case OBJECT_LIFE://回復アイテム
			PlaySoundMem(Data::getInstance()->mLifeItemGetSoundEffectHandle, DX_PLAYTYPE_BACK, TRUE);//アイテム獲得SEの再生
			//修正案。初期化関数を作成して、start関数での初期化を変更する
			itemObject->start();//ヒットしたアイテムオブジェクトの初期化
			(score) += LIFE_SCORE;//アイテム獲得スコアの加算
			//最大の場合効果なし
			if (mLife != MAX_HP)
			{
				//HPを1つ回復
				mIsActiveLifeIcon[mLife] = true;//表示アイコン用のフラグをtrue
				mLife++;
			}
			return OBJECT_LIFE;
			break;

		case OBJECT_STAR://無敵アイテム
			PlaySoundMem(Data::getInstance()->mUnbeatableItemGetSoundEffectHandle, DX_PLAYTYPE_BACK, TRUE);//アイテム獲得SEの再生
			//修正案。初期化関数を作成して、start関数での初期化を変更する
			itemObject->start();//ヒットしたアイテムオブジェクトの初期化
			(score) += STAR_SCORE;//アイテム獲得スコアの加算
			//無敵中の場合、効果時間を最大値にリセット
			if (!mIsUnbeatable)
			{
				mIsUnbeatable = true;
			}
			else
			{
				mUnbeatableCount = 0;
			}
			return OBJECT_STAR;
			break;

		default:
			break;
		}
	}
	return 0;
}
