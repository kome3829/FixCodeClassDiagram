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
	 mWidth= POWER_UP_EF_WIDTH;
	 mHeight= POWER_UP_EF_HEIGHT;
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
	mWidth = POWER_UP_EF_WIDTH;
	mHeight = POWER_UP_EF_HEIGHT;
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

/*
@brief	エフェクトアニメーションの再生位置や種類の設定を行う関数

@param[in]	double* setPositionX	:表示する位置のｘ座標
@param[in]	double* setPositionY	:表示する位置のｙ座標
@param[in]	int		effectType	    :エフェクトアニメーションの種別

@return		なし

@note     引数で渡された位置と種類をもとに、対象のエフェクトを設定する
@note     再生フラグ（mIsPlay）が true の場合は、すでに再生中のため設定を行わない
@note     エフェクトの種類応じて処理を分岐して、サイズ・フレーム数・アニメーションテーブルなどのパラメータを設定する
@note     設定後、再生フラグ（mIsPlay）を true にし、アニメーション再生を開始する状態にする
@warning
- 再生中かどうかを判定できるように、返り値は bool 型としている
- 位置変数は常に更新されるためぽいんたで直接参照している

*/
bool Effect::setEffect(double* setPositionX, double* setPositionY, int effectType)
{
    if (mIsPlay)
    {
        return false;
    }
    // 使用するエフェクト番号を保存する
    mEffectType = effectType;
	//種別に応じてパラメータを設定する
    switch (effectType)
    {
     case POWERUP_EF:
        // パワーアップエフェクトの設定
        mWidth = POWER_UP_EF_WIDTH;
        mHeight = POWER_UP_EF_HEIGHT;
        mSpriteFrameMax = POWER_UP_EF_SPRITE_FRAME_MAX;
        mPlayFrameMax = POWER_UP_EFFECT_FRAME_COUNT_MAX;

        // 再生順テーブルをコピーする
        for (int i = 0; i < mPlayFrameMax; i++)
        {
            mPlayFrameTable[i] = mPowerUpEffectTable[i];
        }
        break;

    case CHARGE_EF:
        // チャージエフェクトの設定
        mWidth = CHARGE_EF_WIDTH;
        mHeight = CHARGE_EF_HEIGHT;
        mSpriteFrameMax = CHARGE_EF_SPRITE_FRAME_MAX;
        mPlayFrameMax = CHARGE_EF_FRAME_COUNT_MAX;
        // 再生順テーブルをコピーする
        for (int i = 0; i < mPlayFrameMax; i++)
        {
            mPlayFrameTable[i] = mChargeEffectTable[i];
        }
        break;

    case HIT_EF:
        // ヒットエフェクトの設定
        mWidth = HIT_EF_WIDTH;
        mHeight = HIT_EF_HEIGHT;
        mSpriteFrameMax = HIT_EF_SPRITE_FRAME_MAX;
        mPlayFrameMax = HIT_EF_FRAME_COUNT_MAX;
        mAlpha = HIT_EF_ALPHA;
        // 再生順テーブルをコピーする
        for (int i = 0; i < mPlayFrameMax; i++)
        {
            mPlayFrameTable[i] = mHitEffectTable[i];
        }
        break;

    case SHOT_EF:
        // 発射エフェクトの設定
        mWidth = SHOT_EF_WIDTH;
        mHeight = SHOT_EF_HEIGHT;
        mSpriteFrameMax = SHOT_EF_SPRITE_FRAME_MAX;
        mPlayFrameMax = SHOT_EF_FRAME_COUNT_MAX;
        // 再生順テーブルをコピーする
        for (int i = 0; i < mPlayFrameMax; i++)
        {
            mPlayFrameTable[i] = mShotEffectTable[i];
        }
        break;

    case LIFE_EF:
        // ライフ取得エフェクトの設定
        mWidth = LIFE_STAR_EF_WIDTH;
        mHeight = LIFE_STAR_EF_HEIGHT;
        mSpriteFrameMax = LIFE_STAR_EF_SPRITE_FRAME_MAX;
        mPlayFrameMax = LIFE_STAR_EF_FRAME_COUNT_MAX;
        // 再生順テーブルをコピーする
        for (int i = 0; i < mPlayFrameMax; i++)
        {
            mPlayFrameTable[i] = mLifeAndStarEffectTable[i];
        }
        break;

    case STAR_EF:
        // スター取得エフェクトの設定
        mWidth = LIFE_STAR_EF_WIDTH;
        mHeight = LIFE_STAR_EF_HEIGHT;
        mSpriteFrameMax = LIFE_STAR_EF_SPRITE_FRAME_MAX;
        mPlayFrameMax = LIFE_STAR_EF_FRAME_COUNT_MAX;
        // 再生順テーブルをコピーする
        for (int i = 0; i < mPlayFrameMax; i++)
        {
            mPlayFrameTable[i] = mLifeAndStarEffectTable[i];
        }
        break;

    case WARP_EF:
        // ワープエフェクトの設定
        mWidth = WARP_EF_WIDTH;
        mHeight = WARP_EF_HEIGHT;
        mSpriteFrameMax = WARP_EF_SPRITE_FRAME_MAX;
        mPlayFrameMax = WARP_EFFRAME_COUNT_MAX;
        // 再生順テーブルをコピーする
        for (int i = 0; i < mPlayFrameMax; i++)
        {
            mPlayFrameTable[i] = mWarpEffectTable[i];
        }
        break;

    case CHARGE_EF_BOSS:
        // ボス用チャージエフェクトの設定
        mWidth = BOSS_CHARGE_EF_WIDTH;
        mHeight = BOSS_CHARGE_EF_HEIGHT;
        mSpriteFrameMax = BOSS_CHARGE_EF_SPRITE_FRAME_MAX;
        mPlayFrameMax = BOSS_CHARGE_EFFRAME_COUNT_MAX;
        // 再生順テーブルをコピーする
        for (int i = 0; i < mPlayFrameMax; i++)
        {
            mPlayFrameTable[i] = mChargeEffectTable[i];
        }
        break;

    default:
        break;
    }
    mX = setPositionX;
    mY = setPositionY;
    mIsPlay = true;
    return true;
}