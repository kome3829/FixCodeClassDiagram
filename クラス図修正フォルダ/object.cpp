#include "object.h"
/*
@brief	コンストラクタ

@param	　　なし
@return		なし

@note      各種初期設定。クラス生成の際に必要なパラメータはすべて設定する

*/
Object::Object()
{
	mX = 0;
	mY = 0;
	mVectorX = OBJECT_VECTOR_X;
	mVectorY = OBJECT_VECTOR_Y;
	mMovementAngle = 0;
	mEXPMoveCount = 0;
	mIsActive = false;
	mItemObjectType = 0;
	mIsActiveHomingMove = false;
	mPlayerX = 0;
	mPlayerY = 0;
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note      クラス内メンバは自動で解放されるので特に記載なし。
*/
Object::~Object()
{
}

/*
@brief	ゲームシーンのメインループで実行する更新処理を行う関数

@param	なし
@return		なし

@note   
- オブジェクトを追従させるか判定を行う
- フラグ(flg)がtrueの場合のみ処理を実行する
- 画面外へ行くとフラグ(flg)をfalseにして無効にする
- アイテムの種類（type）に応じて処理を分岐している

1. OBJECT_EXP
	- 経験値アイテムの移動処理を行う
	- count変数を用いて移動タイミングや動作を制御している
	- 敵から出現する時の移動
	- 出現後プレイヤーに吸い付いてくる移動

2. OBJECT_LIFE
	- 回復アイテムの移動処理を行う
	- 追従フラグ(homing_on)を用いて動作を制御している
	- 徐々に降下していく移動
	- プレイヤーに吸い付いてくる移動

3. OBJECT_STAR
	- 無敵アイテムの移動処理を行う
	- 追従フラグ(homing_on)を用いて動作を制御している
	- 徐々に降下していく移動
	- プレイヤーに吸い付いてくる移動
@warning
- 各アイテムの吸い付き移動はプレイヤーが取りやすいように追加した
*/
void Object::action()
{
	judgeHomingMoveEneble();

	if (mIsActive)
	{
		switch (mItemObjectType)
		{
		case OBJECT_EXP://経験値アイテム

			mEXPMoveCount++;//カウント更新
			//30フレーム後ホーミング開始
			if (mEXPMoveCount >= OBJECT_EXP_HOMING_START_FRAME)
			{
				//逆三角関数(atan2)を使用し、なす角を計算
				double vectorTargetX = (*mPlayerX - mX);
				double vectorTargetY = (*mPlayerY - mY);

				double targetAngle = atan2(vectorTargetY, vectorTargetX);
				//角度から速度を計算
				mVectorX = cos(targetAngle) * OBJECT_ITEM_HOMING_SPEED;
				mVectorY = sin(targetAngle) * OBJECT_ITEM_HOMING_SPEED;
				//位置(mX,mY)に速度(mVectorX,mVectorY)を加算して、移動している
				mX += mVectorX;
				mY += mVectorY;
			}
			//設定された速度に従い移動
			//敵から飛び出てくるような速度をset関数時に設定
			else
			{
				//位置(mX,Ym)に速度(mVectorX,mVectorY)を加算して、移動している
				mX += mVectorX;
				mY += mVectorY;
				//徐々に減速
				mVectorX *= OBJECT_EXP_SPEEDDOWN;
				mVectorY *= OBJECT_EXP_SPEEDDOWN;
			}
			break;

		case OBJECT_LIFE://回復アイテム

			//追従移動
			if (mIsActiveHomingMove)
			{
				//逆三角関数(atan2)を使用し、なす角を計算
				double vectorTargetX = (*mPlayerX - mX);
				double vectorTargetY = (*mPlayerY - mY);
				double targetAngle = atan2(vectorTargetY, vectorTargetX);
				//角度から速度を計算
				mVectorX = cos(targetAngle) * OBJECT_ITEM_HOMING_SPEED;
				mVectorY = sin(targetAngle) * OBJECT_ITEM_HOMING_SPEED;
				//位置(mX,mY)に速度(mVectorX,mVectorY)を加算して、移動している
				mX += mVectorX;
				mY += mVectorY;
			}
			else
			{
				//降下移動
				mY += mVectorY;
			}
			break;

		case OBJECT_STAR://無敵アイテム

			//追従移動
			if (mIsActiveHomingMove)
			{
				//逆三角関数(atan2)を使用し、なす角を計算
				//プレイヤー狙いの角度を計算
				double vectorTargetX = (*mPlayerX - mX);
				double vectorTargetY = (*mPlayerY - mY);
				double targetAngle = atan2(vectorTargetY, vectorTargetX);
				//角度から速度を計算
				mVectorX = cos(targetAngle) * OBJECT_ITEM_HOMING_SPEED;
				mVectorY = sin(targetAngle) * OBJECT_ITEM_HOMING_SPEED;
				//位置(mX,mY)に速度(mVectorX,mVectorY)を加算して、移動している
				mX += mVectorX;
				mY += mVectorY;
			}
			else
			{
				//降下移動
				mY += mVectorY;
			}
			break;

		default:
			break;
		}
		//画面外なら消える処理
		if (mX >= MAX_SCREEN_WIDTH + OBJECT_OUTSIDE_MARGIN ||
			mX <= 0 - OBJECT_OUTSIDE_MARGIN ||
			mY >= MAX_SCREEN_HEIGHT + OBJECT_OUTSIDE_MARGIN ||
			mY <= 0 - OBJECT_OUTSIDE_MARGIN)
		{
			mIsActive = false;
		}
	}
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note      フラグ(flg)がtrueの場合のみ処理を実行する
@note      アイテムの種類（type）に応じて画像表示を切り替えている

*/
void Object::draw()
{
	if (mIsActive)
	{
		//修正案。画像ハンドル変数をを配列にして、スイッチではなく1行で記述にできるのでは？
		switch (mItemObjectType)//種類の応じて画像を表示
		{
		case OBJECT_EXP://経験値アイテム
			DrawGraph((int)mX - ITEM_OBJECT_WIDTH / CUT_HALF, (int)mY - ITEM_OBJECT_HEIGHT / CUT_HALF, Data::getInstance()->mExperiencePointsItemImageHandle, TRUE);
			break;
		case OBJECT_LIFE://回復アイテム
			DrawGraph((int)mX - ITEM_OBJECT_WIDTH / CUT_HALF, (int)mY - ITEM_OBJECT_HEIGHT / CUT_HALF, Data::getInstance()->mLifeItemImageHandle, TRUE);
			break;
		case OBJECT_STAR://無敵アイテム
			DrawGraph((int)mX - ITEM_OBJECT_WIDTH / CUT_HALF, (int)mY - ITEM_OBJECT_HEIGHT / CUT_HALF, Data::getInstance()->mUnbeatableItemImageHandle, TRUE);
			break;

		default:
			break;
		}
	}
}

/*
@brief	動作開始時に位置やフラグなどのパラメータの初期設定を行う関数

@param	なし
@return		なし

*/
void Object::start()
{
	mX = 0;
	mY = 0;
	mPlayerX = 0;
	mPlayerY = 0;
	mVectorX = OBJECT_VECTOR_X;
	mVectorY = OBJECT_VECTOR_Y;
	mMovementAngle = 0;
	mEXPMoveCount = 0;
	mIsActive = false;
	mItemObjectType = 0;
	mIsActiveHomingMove = false;
}

/*
@brief	アイテムドロップに必要な位置やフラグなどのパラメータ設定を行う関数

@param[in]	int setPositionX			:出現させるのX座標
@param[in]	int setPositionY			:出現させるのY座標
@param[in]	int setAngle				:発射角度
@param[in]	int itemObjectType			:アイテムの種類
@param[in]	double* playerPositionX		:プレイヤー位置のｘ座標ポインタ
@param[in]	double* playerPositionY		:プレイヤー位置のｙ座標ポインタ

@return		実行状態:bool:実行した　true/実行してない false

@note      

- フラグ（flg）が true の場合は再設定を行わず false を返す
- フラグ（flg）をtrueにしてaction関数及びdraw関数の処理を有効にしている
- 引数で渡された位置や種類などをもとに各パラメータを設定する
@warning
- ドロップ中か判定できるように、返り値は bool 型としている
-　プレイヤーの位置変数は常に更新されるため、ポインタで直接参照している。
*/
bool Object::setItemObject(int setPositionX, int setPositionY, int setAngle, int itemObjectType ,double* playerPositionX, double* playerPositionY)
{
	if (mIsActive)
	{
		return false;
	}
	mX = setPositionX;
	mY = setPositionY;
	mPlayerX = playerPositionX;
	mPlayerY = playerPositionY;
	mMovementAngle = setAngle;
	mItemObjectType = itemObjectType;
	//経験値アイテムの場合、速度をを設定
	if (itemObjectType == OBJECT_EXP)
	{
		//ラジアンに変換
		double movementRadian = mMovementAngle / DEGREE_TO_RADIAN_DIVISOR * PI;
		mVectorX = cos(movementRadian) * OBJECT_EXP_FAST_SPEED;
		mVectorY = sin(movementRadian) * OBJECT_EXP_FAST_SPEED;
	}
	mIsActive = true;
	return true;
}

/*
@brief	アイテムがプレイヤーを追従するか判定を行う関数

@param[in]	なし
@return		なし

@note      

- 次の条件をすべて満たしたときのみ処理を行う
	- フラグ（flg）が true
	- まだ追従していない（homing_on が false）
	- プレイヤー位置のポインタが nullではない
- プレイヤーに一定距離まで近づいたら追従フラグ（homing_on）を true にする

*/
void Object::judgeHomingMoveEneble()
{
	if (mIsActive && !mIsActiveHomingMove&&mPlayerX!=nullptr && mPlayerY != nullptr)
	{
		//プレイヤーに一定距離まで近づいたら追従フラグ（homing_on）を true にする
		if (abs((int)(mX - *mPlayerX)) < OBJECT_HOMING_RANGE && abs((int)(mY - *mPlayerY)) < OBJECT_HOMING_RANGE)
		{
			mIsActiveHomingMove = true;
		}
	}
}