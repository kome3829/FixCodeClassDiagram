#include "enemybullet.h"

/*
@brief	コンストラクタ

@param	なし
@return		なし

@note	各種初期設定。クラス生成の際に必要なパラメータはすべて設定する
@note	ゴミ値が入らないように変数の初期化を行う
*/
EnemyBullet::EnemyBullet()
{
	mX = 0.0f;
	mY = 0.0f;
	mVectorX = 0.0f;
	mVectorY = 0.0f;
	mAngle = 0.0f;
	mIsActive = false;
	mIsBossEnemy = false;
	mSpeed = ENEMY_BULLET_SPEED;
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note	クラス内メンバは自動で解放されるので特に記載なし
*/
EnemyBullet::~EnemyBullet()
{
}

/*
@brief	ゲームシーンのメインループ内で実行される、敵弾の移動および状態更新処理を行う関数

@param	なし
@return	なし

@note

	- フラグ(flg)がtrueの場合のみ処理を実行する
	- 速度（Vx, Vy）を位置（X, Y）に加算することで弾の移動を行う
	- ボスが発射した弾の場合は移動しながら徐々に速度が減速する
	- 画面外へ行くとstart関数を使用して発射前の状態へ戻す
@warning 
- ボスステージでは速い弾と遅い弾を組み合わせ難易度をあげるため徐々に減速させている
*/
void EnemyBullet::action()
{
	if (mIsBossEnemy)//ボス敵の場合
	{
		if (mIsActive)
		{
			//ラジアンに変換
			double moveRadian = mAngle / DEGREE_TO_RADIAN_DIVISOR * PI;
			//一定値まで徐々に減速
			if (mSpeed >= BOSS_BULLET_SPEEDDOWN_LIMIT)
			{
				mSpeed *= BOSS_BULLET_SPEEDDOWN;
			}
			//角度から速度（Vx,Vy）を計算して設定している
			mVectorX = cos(moveRadian) * mSpeed;
			mVectorY = sin(moveRadian) * mSpeed;
			
			//位置(X,Y)に速度(Vx,Vy)を加算して、移動している
			mX += mVectorX;
			mY += mVectorY;
		}	
		//画面外での処理
		if (mY <= SCREEN_MARGIN || mX <= SCREEN_MARGIN || mY >= MAX_SCREEN_HEIGHT + SCREEN_MARGIN || mX >= MAX_SCREEN_WIDTH + SCREEN_MARGIN)
		{
			start();
		}
	}
	else//雑魚敵の場合
	{
		if (mIsActive)
		{
			//位置(X,Y)に速度(Vx,Vy)を加算して、移動している
			mX += mVectorX;
			mY += mVectorY;
		}
		//画面外での処理
		if (mY <= SCREEN_MARGIN || mX <= SCREEN_MARGIN || mY >= MAX_SCREEN_HEIGHT + SCREEN_MARGIN || mX >= MAX_SCREEN_WIDTH + SCREEN_MARGIN)
		{
			start();
		}
	}
}

/*
@brief	ゲームシーンの描画ループ内で実行される、敵の描画処理を行う関数

@param	なし

@return		なし

@note     フラグ(flg)がtrueの場合のみ処理を実行する
@note     敵弾の画像表示

*/
void EnemyBullet::draw()
{
	
	if (mIsActive)
	{
		DrawGraph((int)mX - ENEMY_BULLET_WIDTH / CUT_HALF, (int)mY - ENEMY_BULLET_HEIGHT / CUT_HALF, Data::getInstance()->mEnemyBulletImageHandle, TRUE);
	}
}

/*
@brief	動作開始時に位置、フラグ、速度などの各パラーメータの初期設定を行う関数。

@param		なし
@return		なし

@note     敵弾の生成時や、画面外に出た際に初期状態へ戻すために使用される
*/
void EnemyBullet::start()
{
	mX = 0.0f;
	mY = 0.0f;
	mVectorX = 0.0f;
	mVectorY = 0.0f;
	mAngle = 0.0f;
	mIsActive = false;
	mIsBossEnemy = false;
	mSpeed = ENEMY_BULLET_SPEED;
}

/*
@brief	敵弾の発射に必要な位置や角度などの設定を行う関数

@param[in]	int 	setPositionX	:出現させるX座標
@param[in]	int 	setPositionY	:出現させるy座標
@param[in]	int 	setAngle		:発射する角度
@param[in]	bool 	isBoss			:ボス判別フラグ

@return		実行判定:bool :実行した　true/実行してない　false 

@note     

- フラグ（flg）が true の場合は再設定を行わず false を返す
- フラグ(flg)をtrueにしてaction関数及びdraw関数の処理を有効にしている
- 引数で渡された位置・角度・ボスフラグをもとに各パラメータを設定する
@warning
- すでに発射中かどうかを判定できるように、返り値は bool 型としている
*/
bool EnemyBullet::setBullet(int setPositionX, int setPositionY, int setAngle,bool isBossEnemy)
{
	if (mIsActive)
	{
		return false;
	}
	mX = setPositionX;
	mY = setPositionY;
	mAngle = setAngle;
	mIsBossEnemy = isBossEnemy;
	//ラジアンに変換
	double moveRadian = mAngle / DEGREE_TO_RADIAN_DIVISOR * PI;
	//ボスと通常敵で場合の設定する初速を切り替え
	if (mIsBossEnemy)
	{
		//角度から速度（Vx,Vy）を計算して設定している
		mVectorX = cos(moveRadian) * BOSS_BULLET_FAST_SPEED;
		mVectorY = sin(moveRadian) * BOSS_BULLET_FAST_SPEED;
	}
	else
	{
		//角度から速度（Vx,Vy）を計算して設定している
		mVectorX = cos(moveRadian) * ENEMY_BULLET_FAST_SPEED;
		mVectorY = sin(moveRadian) * ENEMY_BULLET_FAST_SPEED;
	}
	mIsActive = true;
	return true;
}
