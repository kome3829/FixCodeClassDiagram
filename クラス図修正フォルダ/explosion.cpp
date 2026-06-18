#include "explosion.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note	各種初期設定。クラス生成の際に必要なパラメータはすべて設定する
@note	ゴミ値が入らないように変数の初期化を行う
*/
Explosion::Explosion()
{
     mX = 0;
	 mY = 0;
	 mCurrentPlayFrameNumber = 0;
	 mSpriteFrameX = 0;
	 mSpriteFrameY = 0;
	 mFrameCount = 0;
	 mWidth = EXPLOSION_WIDTH;
	 mHeight = EXPLOSION_HEIGHT;
	 mIsPlay = false;
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note	クラス内メンバは自動で解放されるので特に記載なし
*/
Explosion::~Explosion()
{
}

/*
@brief	爆破エフェクトのアニメーション更新および描画処理を行う関数

@param	なし
@return		なし

@note      再生フラグ（Playcheck）が true の場合のみ処理を実行する
@note      アニメーションカウントを更新し、テーブル（animeTable）に基づいて表示するフレームを決定する
@note      フレーム番号から描画位置を計算し、スプライトシートから画像を切り出して指定位置に描画する
@note      アニメーションが終了すると、再生フラグ（Playcheck）を false にし、カウントをリセットする
*/
void Explosion::playExplosionAnimation()
{
	if (mIsPlay)
	{
		//テーブルから表示するコマを設定
		 mCurrentPlayFrameNumber = mExplosionPlayFrameTable[mFrameCount];
		 //スプライト分割数に応じだ番号に変更
		 mSpriteFrameX = mCurrentPlayFrameNumber % EXPLOSION_SPRITE_COL; 
		 mSpriteFrameY = mCurrentPlayFrameNumber / EXPLOSION_SPRITE_COL; 
		DrawRectGraph(mX - mWidth / CUT_HALF, mY - mHeight / CUT_HALF, mWidth * mSpriteFrameX, mHeight * mSpriteFrameY, mWidth, mHeight, Data::getInstance()->mBlueExplosionHandle, true, FALSE);
		mFrameCount++;//次のフレームへ更新
		//修正案　判別を　>=に変更
		if (mFrameCount > EXPLOSION_ANIME_END)
		{
			mIsPlay = false;
			mFrameCount = 0;      
		}
	}
}

/*
@brief	動作開始時に位置や再生フラグなどの各パラメータの初期設定を行う関数。

@param		なし

@return		なし

*/
void Explosion::start()
{
	mX = 0;
	mY = 0;
	mCurrentPlayFrameNumber = 0;
	mSpriteFrameX = 0;
	mSpriteFrameY = 0;
	mFrameCount = 0;
	mWidth = EXPLOSION_WIDTH;
	mHeight = EXPLOSION_HEIGHT;
	mIsPlay = false;
}

/*
@brief	爆破エフェクトアニメーションの再生位置などを設定する関数

@param[in]	int setPositionX	:再生位置のX座標
@param[in]	int setPositionY	:再生位置のY座標

@return		実行判定:bool :実行した　true/実行してない　false 

@note     

  - 再生フラグ（Playcheck）が true の場合は、すでに再生中のため設定を行わず false を返す
  - 引数から再生する位置を設定する
  - 設定後、再生フラグ（Playcheck）を true にし、アニメーション再生を開始する状態にする
@warning
  - 再生中かどうかを判定できるように、返り値は bool 型としている
*/
bool Explosion::setExplosion(int setPositionX, int setPositionY)
{
	if (mIsPlay)
	{
		return false;
	}
	mX = setPositionX;
	mY = setPositionY;
	mIsPlay = true;
	return true;
}
