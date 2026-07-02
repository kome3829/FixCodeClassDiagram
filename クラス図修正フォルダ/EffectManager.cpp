#include "EffectManager.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note      エフェクトクラスのインスタンスを生成している

*/
EffectManager::EffectManager()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
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
@brief	処理開始に必要なパラメータの初期設定や処理を行う関数

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
@note     設定後、再生フラグ（mIsPlay）を
trueにし、アニメーション再生を開始する状態にする
@warning
- 位置変数はポインタで直接参照している

*/
void EffectManager::setEffect(double *setPositionX, double *setPositionY,
                              int effectType)
{
	// フラグのたっていないものを探し、1つだけsetEffect関数を実行
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		if (mEffects[i]->mIsPlay)
		{
			continue;
		}

		// 使用するエフェクト番号を保存する
		mEffects[i]->mEffectType = effectType;
		//パラメータの設定
		setParameter(mEffects[i], mEffectPlayFrameTableList[effectType],
		             effectType);
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
	// 各パラメータの設定
	effect->mWidth = EFFECT_DATA[effectType].mWidth;
	effect->mHeight = EFFECT_DATA[effectType].mHeight;
	effect->mSpriteFrameMax = EFFECT_DATA[effectType].mSpriteFrameMax;
	effect->mPlayFrameMax = EFFECT_DATA[effectType].mPlayFrameMax;
	if (effectType == HIT_EF)//ヒットエフェクトだけ透過度を変更しているため
	{
		effect->mAlpha = HIT_EF_ALPHA;
	}
	// 再生順テーブルをコピーする
	for (int i = 0; i < effect->mPlayFrameMax; i++)
	{
		effect->mPlayFrameTable[i] = playFrameTable[i];
	}
}
