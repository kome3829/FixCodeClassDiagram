#include "HomingBullet.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note      各種初期設定。クラス生成の際に必要なパラメータはすべて設定する

*/
HomingBullet::HomingBullet()
{
	mX = 0.0f;
	mY = 0.0f;
	mVectorX = 0.0f;
	mVectorY = 0.0f;
	mAngle = 0.0f;
	mIsActive = false;
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note      クラス内メンバは自動で解放されるので特に記載なし。
*/
HomingBullet::~HomingBullet()
{
}

/*
@brief	ゲームシーンのメインループで実行する更新処理を行う関数

@param	なし
@return		なし

@note      フラグ(flg)がtrueの場合のみ処理を実行する
@note      位置(X,Y)に速度(Vx,Vy)を加算して、移動している

*/
void HomingBullet::action()
{
	if (mIsActive)
	{
		//位置(X,Y)に速度(Vx,Vy)を加算して、移動している
		mX += mVectorX;
		mY += mVectorY;
	}
	//敵と自分の座標から角度を出す
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note      フラグ(flg)がtrueの場合のみ処理を実行する。
@note      ホーミング弾の画像を表示する。

*/
void HomingBullet::draw()
{
	if (mIsActive)
	{
		DrawGraph((int)mX  - PLAYER_BULLET_WIDTH / CUT_HALF, (int)mY  -  PLAYER_BULLET_HEIGHT / CUT_HALF -  HOMING_DRAW_OFFSET_Y, Data::getInstance() -> mPlayerBulletImageHandleArray[BULLET_NORMAL], true);
	}
}

/*
@brief	動作開始時に位置やフラグなどのパラメータの初期設定を行う関数

@param	なし
@return		なし

*/
void HomingBullet::start()
{
	mX = 0.0f;
	mY = 0.0f;
	mVectorX = 0.0f;
	mVectorY = 0.0f;
	mAngle = 0.0f;
	mIsActive = false;
}

/*
@brief	弾の発射に必要な位置や角度などの設定を行う関数

@param[in]	int setPositionX	:出現させるのX座標
@param[in]	int setPositionY	:出現させるのY座標
@param[in]	int setAngle		:発射角度

@return		実行状態:bool:実行した　true/実行してない false

@note      

- フラグ（flg）が true の場合は再設定を行わず false を返す
- フラグ(flg)をtrueにしてaction関数及びdraw関数の処理を有効にしている
- 引数で渡された位置・角度もとに各パラメータを設定する
@warning
- すでに発射中かどうかを判定できるように、返り値は bool 型としている
*/
bool HomingBullet::setBullet(int setPositionX, int setPositionY, int setAngle)
{
	if (mIsActive)
	{
		return false;
	}
	//発射SEの再生
	PlaySoundMem(Data::getInstance()->mPlayerShotSoundEffectHandle, DX_PLAYTYPE_BACK, TRUE);
	mX = setPositionX;
	mY = setPositionY;
	//修正案。mAngleの削除もしくはローカル変数
	mAngle = setAngle;
	//角度から速度を計算
	mVectorX =  cos(mAngle) *HOMING_BULLET_SPEED;
	mVectorY = sin(mAngle) * HOMING_BULLET_SPEED;
	mIsActive = true;
	return true;
}

