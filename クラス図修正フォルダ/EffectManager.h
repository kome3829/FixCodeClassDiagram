#ifndef __EFFECTMANAGER_H__
#define __EFFECTMANAGER_H__

#include "Deta.h"
#include "DxLib.h"
#include "Effect.h"
#include "define.h"

struct EffectData
{
	int mWidth;          // 横サイズ
	int mHeight;         // 縦サイズ
	int mSpriteFrameMax; // スプライト画像の横の分割数
	int mPlayFrameMax;   // 最大再生フレーム数
};
// 各エフェクトのパラメータ
constexpr EffectData EFFECT_DATA[] = {
    {POWER_UP_EF_WIDTH, POWER_UP_EF_HEIGHT, POWER_UP_EF_SPRITE_FRAME_MAX,
     POWER_UP_EF_FRAME_COUNT_MAX}, // POWER UP
    {CHARGE_EF_WIDTH, CHARGE_EF_HEIGHT, CHARGE_EF_SPRITE_FRAME_MAX,
     CHARGE_EF_FRAME_COUNT_MAX}, // CHARGE
    {HIT_EF_WIDTH, HIT_EF_HEIGHT, HIT_EF_SPRITE_FRAME_MAX,
     HIT_EF_FRAME_COUNT_MAX}, // HIT
    {SHOT_EF_WIDTH, SHOT_EF_HEIGHT, SHOT_EF_SPRITE_FRAME_MAX,
     SHOT_EF_FRAME_COUNT_MAX}, // SHOT
    {LIFE_STAR_EF_WIDTH, LIFE_STAR_EF_HEIGHT, LIFE_STAR_EF_SPRITE_FRAME_MAX,
     LIFE_STAR_EF_FRAME_COUNT_MAX}, // LIFE/STAR共通
    {LIFE_STAR_EF_WIDTH, LIFE_STAR_EF_HEIGHT, LIFE_STAR_EF_SPRITE_FRAME_MAX,
     LIFE_STAR_EF_FRAME_COUNT_MAX}, // LIFE/STAR共通
    {WARP_EF_WIDTH, WARP_EF_HEIGHT, WARP_EF_SPRITE_FRAME_MAX,
     WARP_EFFRAME_COUNT_MAX}, // WARP
    {BOSS_CHARGE_EF_WIDTH, BOSS_CHARGE_EF_HEIGHT,
     BOSS_CHARGE_EF_SPRITE_FRAME_MAX,
     BOSS_CHARGE_EFFRAME_COUNT_MAX}, // BOSS CHARGE
    {EXPLOSION_WIDTH, EXPLOSION_HEIGHT, EXPLOSION_SPRITE_COL,
     EXPLOSION_ANIME_END} // EXPLOSION

};



class EffectManager
{
  public:
	EffectManager();
	~EffectManager();
	void start();
	// エフェクトアニメーション再生
	void playEffectAnimation();
	// エフェクトアニメーションの設置
	void setEffect(double *setPositionX, double *setPositionY, int effectType);
	// パラメータの設定
	void setParameter(Effect *effect, const int playFrameTable[],
	                  int effectType);
	// エフェクトクラス
	Effect *mEffects[MAX_BULLET_NUMBER];

  private:

	//---各エフェクトの再生するフレーム番号を順番に並べたテーブル配列---
	static constexpr int mPowerUpEffectTable
	    [POWER_UP_EF_FRAME_COUNT_MAX] = // パワーアップエフェクトの再生順テーブル
	    {0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7};

	static constexpr int mChargeEffectTable
	    [MAX_TABLE_FRAME] = // チャージエフェクトの再生順テーブル
	    {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6,
	     6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3,
	     3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9,
	     0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6,
	     6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3,
	     3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9};

	static constexpr int mHitEffectTable
	    [HIT_EF_FRAME_COUNT_MAX] = // ヒットエフェクトの再生順テーブル
	    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

	static constexpr int mShotEffectTable
	    [SHOT_EF_FRAME_COUNT_MAX] = // 発射エフェクトの再生順テーブル
	    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

	static constexpr int mLifeAndStarEffectTable
	    [LIFE_STAR_EF_FRAME_COUNT_MAX] = // 回復及び無敵アイテム取得エフェクトの再生順テーブル
	    {0, 0, 1, 1, 2, 2,  3,  3,  4,  4,  5,  5,  6,  6,  7,
	     7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14};

	static constexpr int mWarpEffectTable
	    [WARP_EFFRAME_COUNT_MAX] = // ワープエフェクトの再生順テーブル
	    {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
	     13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

	static constexpr int mExplosionPlayFrameTable
	    [EXPLOSION_ANIME_END] = // 爆発エフェクトの再生順テーブル
	    {0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7};


	//再生順テーブルをまとめた配列
	static constexpr const int *mEffectPlayFrameTableList[EFFECT_TYPE_MAX] = {
	    mPowerUpEffectTable, mChargeEffectTable,      mHitEffectTable,
	    mShotEffectTable,    mLifeAndStarEffectTable, mLifeAndStarEffectTable,
	    mWarpEffectTable,    mChargeEffectTable,      mExplosionPlayFrameTable};
	//----------------------------------
};
#endif // !__EFFECTMANAGER_H__
