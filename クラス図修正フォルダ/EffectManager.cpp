#include "EffectManager.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note      エフェクトクラスのインスタンスを生成している

*/
EffectManager::EffectManager()
{
	for (int i = 0; i < MAX_BULLET_NUMBER;i++)
	{
		mEffects[i] = new Effect();
	}
}
/*
@brief	デストラクタ

@param	なし
@return		なし

@note
インスタンス生成した エフェクトクラスのdeleteを行い、メモリの開放を行う
*/
EffectManager::~EffectManager()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		delete mEffects[i];
		mEffects[i] = nullptr;

	}
}
/*
@brief	処理開始に必要なパラーメータの初期設定や処理を行う関数

@param	なし
@return		なし
@note	生成した全てのエフェクトクラスの初期設定や処理を行う

*/
void EffectManager::start()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mEffects[i]->start();
	}
}

/*
@brief	エフェクトアニメーションの更新および描画処理を行う関数

@param		なし
@return		なし

@note     生成した全てのエフェクトクラスの更新および描画処理を行う
*/
void EffectManager::playEffectAnimation()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mEffects[i]->playEffectAnimation();
	}
}
/*
@brief	エフェクトアニメーションの再生位置や種類の設定を行う関数

@param[in]	double* setPositionX	:表示する位置のｘ座標
@param[in]	double* setPositionY	:表示する位置のｙ座標
@param[in]	int		effectType	    :エフェクトアニメーションの種別

@return		なし

@note     引数で渡された位置と種類をもとに、対象のエフェクトを設定する
@note     再生フラグ（mIsPlay）が trueの場合は、すでに再生中のため設定を行わない
@note
エフェクトの種類応じて処理を分岐して、サイズ・フレーム数・アニメーションテーブルなどのパラメータを設定する
@note     設定後、再生フラグ（mIsPlay）を trueにし、アニメーション再生を開始する状態にする
@warning
- 位置変数は常に更新されるためぽいんたで直接参照している

*/
void EffectManager::setEffect(double *setPositionX, double *setPositionY,
                              int effectType)
{
	// フラグのたっていないものを探し、1つだけsetbullet関数を実行
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		if (mEffects[i]->mIsPlay)
		{
			continue;
		}

		// 使用するエフェクト番号を保存する
		mEffects[i]->mEffectType = effectType;
		// 種別に応じてパラメータを設定する
		switch (effectType)
		{
		case POWERUP_EF:
			// パワーアップエフェクトの設定
			setParameter(mEffects[i], mPowerUpEffectTable, POWERUP_EF);
				break;

		case CHARGE_EF:
			// チャージエフェクトの設定
			setParameter(mEffects[i], mChargeEffectTable, CHARGE_EF);

			break;

		case HIT_EF:
			// ヒットエフェクトの設定
			setParameter(mEffects[i], mHitEffectTable, HIT_EF);
			mEffects[i]->mAlpha = HIT_EF_ALPHA;
			break;

		case SHOT_EF:
			// 発射エフェクトの設定
			setParameter(mEffects[i], mShotEffectTable, SHOT_EF);
			break;

		case LIFE_EF:
			// ライフ取得エフェクトの設定
			setParameter(mEffects[i], mLifeAndStarEffectTable, LIFE_EF);
			break;

		case STAR_EF:
			// スター取得エフェクトの設定
			setParameter(mEffects[i], mLifeAndStarEffectTable, STAR_EF);
			break;

		case WARP_EF:
			// ワープエフェクトの設定
			setParameter(mEffects[i], mWarpEffectTable, WARP_EF);
			break;

		case CHARGE_EF_BOSS:
			// ボス用チャージエフェクトの設定
			setParameter(mEffects[i], mChargeEffectTable, CHARGE_EF_BOSS);
			break;

		case EXPLOSION_EF:
			// ボス用チャージエフェクトの設定
			setParameter(mEffects[i], mExplosionPlayFrameTable, EXPLOSION_EF);
			break;
		default:
			break;
		}
		mEffects[i]->mX = setPositionX;
		mEffects[i]->mY = setPositionY;
		mEffects[i]->mIsPlay = true;
		return;
	}
}


/*
@brief	エフェクトアニメーションの各パラメータ設定を行う関数

@param[in]	Effect *effect	:設定を行うクラスポインタ
@param[in]	const int playFrameTable[]	:再生順テーブル
@param[in]	int		effectType	    :エフェクトアニメーションの種別

@return		なし

@note     引数で渡された対象のエフェクトを設定する
@note
エフェクトの種類応じた、サイズ・フレーム数・アニメーションテーブルなどのパラメータを設定する
*/
void EffectManager::setParameter(Effect *effect, const int playFrameTable[],
                                 int effectType)
{
	//各パラメータの設定
	effect->mWidth = mEffectWidthList[effectType];
	effect->mHeight = mEffectHightList[effectType];
	effect->mSpriteFrameMax = mEffectSpriteFrameMaxList[effectType];
	effect->mPlayFrameMax = mEffectPlayFrameMaxList[effectType];

	// 再生順テーブルをコピーする
	for (int i = 0; i < effect->mPlayFrameMax; i++)
	{
		effect->mPlayFrameTable[i] = playFrameTable[i];
	}
}
