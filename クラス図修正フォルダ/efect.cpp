#include "efect.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note	各種初期設定。クラス生成の際に必要なパラメータはすべて設定する
@note	ゴミ値が入らないように変数の初期化を行う
*/
Effect::Effect()
{
	 mX=0;
	 mY=0;
	 mCurrentPlayFrameNumber=0;
	 mSpriteFrameX=0;
	 mSpriteFrameY=0;
	 mFrameCount=0;
	 mWidth= 0;
	 mHeight= 0;
	 mEffectType = 0;
	 mIsPlay=false;
     mPlayFrameMax = 0;
	 mSpriteFrameMax = 0;
	 mAlpha = MAX_ALPHA;
     for (int i = 0;i < MAX_TEBLE_FRAME; i++)
     {
         mPlayFrameTable[i] = 0;
     }
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note	クラス内メンバは自動で解放されるので特に記載なし
*/
Effect::~Effect()
{
}

/*
@brief	動作開始時に必要な各種パラメータの初期設定を行う関数

@param		なし
@return		なし

@note     アニメーション再生終了後や、状態を初期状態に戻す際にも使用される。
*/
void Effect::start()
{
	mX = 0;
	mY = 0;
	mCurrentPlayFrameNumber = 0;
	mSpriteFrameX = 0;
	mSpriteFrameY = 0;
	mFrameCount = 0;
	mWidth = 0;
	mHeight = 0;
	mEffectType = 0;
	mSpriteFrameMax = 0;
	mPlayFrameMax = 0;
	mAlpha = MAX_ALPHA;
	mIsPlay = false;
}

/*
@brief	エフェクトアニメーションの更新および描画処理を行う関数

@param		なし
@return		なし

@note     再生フラグ（Playcheck）が true の場合のみ処理を実行する
@note     アニメーションカウントを更新し、テーブルに基づいて表示するフレームを決定する
@note     指定された位置に画像を描画する
@note     アニメーションが終了すると、start 関数を呼び出して状態を初期化する
*/
void Effect::playEffectAnimation()
{
	// 再生フラグが有効な場合のみアニメーション処理を行う
	if (mIsPlay)
	{
		// 現在の再生フレーム番号をテーブルから取得する
		mCurrentPlayFrameNumber = mPlayFrameTable[mFrameCount];
		// スプライトシート上の横方向のコマ番号を計算する
		mSpriteFrameX = mCurrentPlayFrameNumber % mSpriteFrameMax;
		// スプライトシート上の縦方向のコマ番号を計算する
		mSpriteFrameY = mCurrentPlayFrameNumber / mSpriteFrameMax;
		// アルファ値を設定して半透明描画を有効にする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, mAlpha);
		// 現在座標を中心にして該当コマを描画する
		DrawRectGraph(
			 (int)(*mX) - mWidth / CUT_HALF,
             (int)(*mY) - mHeight / CUT_HALF,
			mWidth * mSpriteFrameX,                
			mHeight * mSpriteFrameY,                
			mWidth, mHeight,                   
			Data::getInstance()->mEffectAnimationHandleArray[mEffectType], 
			true,                   
			FALSE
		);
		// 描画ブレンド設定を通常状態に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// 次のフレームへ進める
		mFrameCount++;
		// 再生フレーム数が上限に達したら初期状態へ戻す
		if (mFrameCount >= mPlayFrameMax)
		{
			start();
		}
	}
}


