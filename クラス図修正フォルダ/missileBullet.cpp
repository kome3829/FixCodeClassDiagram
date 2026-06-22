#include "missileBullet.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note      各種初期設定。クラス生成の際に必要なパラメータはすべて設定する

*/
MissileBullet::MissileBullet()
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
MissileBullet::~MissileBullet()
{
}

/*
@brief	ゲームシーンのメインループで実行する更新処理を行う関数

@param	なし
@return		なし

@note      フラグ(flg)がtrueの場合のみ処理を実行する
@note      位置(X,Y)に速度(Vx,Vy)を加算して、移動している
@note      画面外へ行くとstart関数を実行して発射前の状態へ戻している

*/
void MissileBullet::action()
{
	if (mIsActive)
	{
		//位置(X,Y)に速度(Vx,Vy)を加算して、移動している
		mX += mVectorX;
		mY += mVectorY;
	}
	//画面外判定
	if (mY <= SCREEN_MARGIN || mX <= SCREEN_MARGIN || mY >= MAX_SCREEN_HEIGHT + SCREEN_MARGIN || mX >= MAX_SCREEN_WIDTH + SCREEN_MARGIN)
	{
		start();
	}
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note      フラグ(flg)がtrueの場合のみ処理を実行する
@note      ミサイル弾弾の画像を表示する

*/
void MissileBullet::draw()
{
	if (mIsActive)
	{
		//修正案。オフセットの削除
		DrawGraph((int)mX - PLAYER_BULLET_WIDTH / CUT_HALF, (int)mY - PLAYER_BULLET_HEIGHT / CUT_HALF - MISSILE_DRAW_OFFSET_Y, Data::getInstance()->mPlayerBulletImageHandleArray[PLAYER_MISSILE], true);
	}
}

/*
@brief	動作開始時に位置やフラグなどのパラメータの初期設定を行う関数

@param	なし
@return		なし

*/
void MissileBullet::start()
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
bool MissileBullet::setBullet(int setPositionX, int setPositionY, int setAngle)
{
	if (mIsActive)
	{
		return false;
	}
	//発射SEの再生
	PlaySoundMem(Data::getInstance()->mPlayerShotSoundEffectHandle, DX_PLAYTYPE_BACK, TRUE);

	mX = setPositionX;
	mY = setPositionY;
	//180度反転
	//修正案。mAngleの削除もしくはローカル変数にする
	mAngle = setAngle + DEGREE_HALF_TURN;
	//ラジアンに変換
	double moveRadian = mAngle / DEGREE_TO_RADIAN_DIVISOR * PI;
	//角度から速度を計算
	mVectorX = cos(moveRadian) * MISAAILE_BULLET_SPD;
	mVectorY = sin(moveRadian) * MISAAILE_BULLET_SPD;
	mIsActive = true;
	return true;
}
