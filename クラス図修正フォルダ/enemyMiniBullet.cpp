#include "enemyMiniBullet.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note	各種初期設定。クラス生成の際に必要なパラメータはすべて設定する
@note	ゴミ値が入らないように変数の初期化を行う
*/
EnemyMiniBullet::EnemyMiniBullet()
{
	mX = 0.0f;
	mY = 0.0f;
	mVectorX = 0.0f;
	mVectorY = 0.0f;
	mAngle = 0.0f;
	mIsActive = false;
	mImageType = 0;
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note	クラス内メンバは自動で解放されるので特に記載なし
*/
EnemyMiniBullet::~EnemyMiniBullet()
{
}

/*
@brief	ゲームシーンの描画ループ内で実行される、敵ミニ弾の描画処理を行う関数

@param	なし

@return		なし

@note      ゲームシーンの描画ループ内で実行される、敵ミニ弾の描画処理を行う関数
@note      フラグ(flg)がtrueの場合のみ処理を実行する
@note      弾の種類（Type）に応じて表示する画像を切り替え、描画する

*/
void EnemyMiniBullet::draw()
{
	if (mIsActive)
	{
		DrawGraph((int)mX - ENEMY_BULLET_MINI_WIDTH / CUT_HALF, (int)mY - ENEMY_BULLET_MINI_HEIGHT / CUT_HALF, Data::getInstance()->mEnemyMiniBulletImageHandleArray[mImageType], TRUE);
	}
}

/*
@brief	敵のミニ弾の発射に必要な位置や角度などの設定を行う関数

@param[in]	int 	setPositionX	:出現させるX座標
@param[in]	int 	setPositionY	:出現させるy座標
@param[in]	int 	setAngle		:発射する角度
@param[in]	int 	imageType		:表示画像の種類

@return		実行判定:bool :実行した　true/実行してない　false 

@note     

   - フラグ(flg)をtrueにしてdraw関数及びaction関数の処理を有効にしている
   - フラグ（flg）が true の場合は再設定を行わず false を返す
   - 引数で渡された位置、角度、弾の種類をもとに各パラメータの設定を行う
@warning
   - すでには発射中か判別できるように返り値をbool型としている
*/
bool EnemyMiniBullet::setBullet(int setPositionX, int setPositionY, double setAngle, int imageType)
{
	if (mIsActive)
	{
		return false;
	}
	mImageType = imageType;
	mX = setPositionX;
	mY = setPositionY;
	mAngle = setAngle;
	//修正案。mAngle、ローカル変数の削除。setAngleそのまま代入
	double moveRadian = mAngle;
	//三角関数を使用しx軸、y軸の速度を設定
	mVectorX = cos(moveRadian) * ENEMY_MINI_BULLET_FAST_SPEED;
	mVectorY = sin(moveRadian) * ENEMY_MINI_BULLET_FAST_SPEED;
	mIsActive = true;
	return true;
}

/*
@brief	動作開始時に位置、フラグ、速度などの各パラーメータの初期設定を行う関数。

@param		なし
@return		なし

@note     敵ミニ弾の生成時や、画面外に出た際に初期状態へ戻すために使用される
*/
void EnemyMiniBullet::start()
{
	mX = 0.0f;
	mY = 0.0f;
	mVectorX = 0.0f;
	mVectorY = 0.0f;
	mAngle = 0.0f;
	mIsActive = false;
	mImageType = 0;
}

