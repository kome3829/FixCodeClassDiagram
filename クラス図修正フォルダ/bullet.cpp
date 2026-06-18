#include "bullet.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note	各種初期設定。クラス生成の際に必要なパラメータはすべて設定する
@note	ゴミ値が入らないように変数の初期化を行う
*/
Bullet::Bullet()
{
	 mX = 0.0f;
	 mY = 0.0f;
	 mVectorX = 0.0f;
	 mVectorY = 0.0f;
	 mAngle = 0.0f;
	 mIsActive=false;
}


/*
@brief	デストラクタ

@param	なし
@return		なし

@note	クラス内メンバは自動で解放されるので特に記載なし
*/
Bullet::~Bullet()
{
}

/*
@brief	メインループで実行する更新処理を行う関数

@param     	なし
@return     なし
@note		フラグ（flg）がtrueの場合のみ処理を実行する
@note		位置(X,Y)に速度(Vx,Vy)を加算して、移動している
@note		画面外へ行くとstart関数を実行して発射前の状態へ戻している
*/
void Bullet::action()
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
		start();//修正案。初期化関数をコピーしない
	}
}

/*
@brief	描画ループで実行する描画処理を行う関数

@param[in]	なし
@return		なし

@note	フラグ(flg)がtrueの場合のみ処理を実行する
@note	通常弾の画像を2つ表示する
@warning	迫力に欠けるため表示だけ2つにしている


*/
void Bullet::draw()
{
	if (mIsActive)
	{
		DrawGraph((int)mX - PLAYER_BULLET_WIDTH / CUT_HALF - DRAW_OFFSET_X, (int)mY - PLAYER_BULLET_HEIGHT / CUT_HALF - DRAW_OFFSET_Y, Data::getInstance()->mPlayerBulletImageHandleArray[BULLET_NORMAL], true);
		DrawGraph((int)mX - PLAYER_BULLET_WIDTH / CUT_HALF + DRAW_OFFSET_X, (int)mY - PLAYER_BULLET_HEIGHT / CUT_HALF - DRAW_OFFSET_Y, Data::getInstance()->mPlayerBulletImageHandleArray[BULLET_NORMAL], true);
	}
}

/*
@brief	処理開始に必要なパラーメータの初期設定や処理を行う関数

@param[in]	なし
@return		なし
*/
void Bullet::start()
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

@param[in]	int setpositionX:設定するx座標
@param[in]	int setpositionY:設定するy座標
@param[in]	int setAngle:設定する角度

@return		実行の成否
@note   実行時 ture/ 失敗時 false	
@note	フラグ（flg）が true の場合は再設定を行わず false を返す
@note	フラグ(flg)をtrueにしてaction関数及びdraw関数の処理を有効にしている
@note	引数で渡された位置・角度をもとに各パラメータを設定する	
@warning

- すでに発射中かどうかを判定できるように、返り値は bool 型としている

- ※角度から速度を設定するが、通常弾は画面上向きで発射するので角度の引数は使用していない
*/
bool Bullet::setBullet(int setPositionX, int setPositionY, int setAngle)
{
	if (mIsActive)
	{
		return false;
	}
	//発射SEの再生
	PlaySoundMem(Data::getInstance()->mPlayerShotSoundEffectHandle, DX_PLAYTYPE_BACK, TRUE);

	//各パラメータの設定
	mX = setPositionX;
	mY = setPositionY;
	//修正案。mAngleの削除もしくはローカル変数
	mAngle = setAngle;
	mVectorX = 0;
	mVectorY = PLAYER_BULLET_VECTOR_Y;
	mIsActive = true;
	return true;
}
