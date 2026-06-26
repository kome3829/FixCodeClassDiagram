#include"Bullet.h"
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
	mIsActive = false;
	mWidth = 0;
	mHeight = 0;
	mBulletType = PLAYER_NOMAL;
	mIsBossEnemy = false;
	mSpeed = ENEMY_BULLET_SPEED;
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
	if (!mIsActive)
	{
		return;
	}
	if (mIsBossEnemy) // ボス敵の場合
	{
		// ラジアンに変換
		double moveRadian = mAngle / DEGREE_TO_RADIAN_DIVISOR * PI;
		// 一定値まで徐々に減速
		if (mSpeed >= BOSS_BULLET_SPEEDDOWN_LIMIT)
		{
			mSpeed *= BOSS_BULLET_SPEEDDOWN;
		}
		// 角度から速度（Vx,Vy）を計算して設定している
		mVectorX = cos(moveRadian) * mSpeed;
		mVectorY = sin(moveRadian) * mSpeed;

		// 位置(X,Y)に速度(Vx,Vy)を加算して、移動している
		mX += mVectorX;
		mY += mVectorY;

	}
	else
	{
		// 位置(X,Y)に速度(Vx,Vy)を加算して、移動している
		mX += mVectorX;
		mY += mVectorY;
	}
	// 画面外での処理
	if (mY <= SCREEN_MARGIN || mX <= SCREEN_MARGIN ||
	    mY >= MAX_SCREEN_HEIGHT + SCREEN_MARGIN ||
	    mX >= MAX_SCREEN_WIDTH + SCREEN_MARGIN)
	{
		start();
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
	if (!mIsActive)
	{
		return;
	}

	DrawGraph((int)mX - mWidth / CUT_HALF, (int)mY - mHeight / CUT_HALF,
	          Data::getInstance()->mBulletImgageHandleArray[mBulletType], true);

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
	mIsBossEnemy = false;
	mSpeed = ENEMY_BULLET_SPEED;
	mBulletType = PLAYER_NOMAL;
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

-
※角度から速度を設定するが、通常弾は画面上向きで発射するので角度の引数は使用していない
*/
/*
@brief	弾の当たり判定を行う関数

@param[in]	Character *class_p:当たり判定をとるクラスのポインタ

@note	弾と当たり判定を行うクラスとの距離で距離で判定
@note	引数でクラスポインタから座標とサイズを参照している
*/
bool Bullet::hitCheck(Character *class_p)
{
	if (!mIsActive||!class_p->mIsActive)
	{
		return false;
	}
	// 弾と当たり判定をとるクラスとの距離を計算
	int distX = abs((int)(class_p->mX - mX));
	int distY = abs((int)(class_p->mY - mY));

	// 弾と当たり判定をとるクラスのサイズを加算した半分の値を計算
	int hitRangeX = (class_p->mWidth + mWidth) / 2;
	int hitRangeY = (class_p->mHeight + mHeight) / 2;

	// 一定距離まで近づいたら当たったと判定
	if (distX < hitRangeX && distY < hitRangeY)
	{
		// 弾クラスの無効化
		mIsActive = false;
		if (!class_p->mIsTakeDamage)//ダメージ判定中ではなければ
		{
			class_p->mIsTakeDamage = true; // ダメージ判定を有効
		}
		return true;
	}
	return false;
}

