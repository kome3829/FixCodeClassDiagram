#ifndef __EFFECTMANAGER_H__
#define __EFFECTMANAGER_H__

#include "Deta.h"
#include "DxLib.h"
#include "Effect.h"
#include "define.h"

// --- POWER UP ---
#define POWER_UP_EF_WIDTH (150)
#define POWER_UP_EF_HEIGHT (150)
#define POWER_UP_EF_SPRITE_FRAME_MAX (8)
#define POWER_UP_EFFECT_FRAME_COUNT_MAX (20)

// --- CHARGE ---
#define CHARGE_EF_WIDTH (120)
#define CHARGE_EF_HEIGHT (120)
#define CHARGE_EF_SPRITE_FRAME_MAX (10)
#define CHARGE_EF_FRAME_COUNT_MAX (40)

// --- HIT ---
#define HIT_EF_WIDTH (120)
#define HIT_EF_HEIGHT (120)
#define HIT_EF_SPRITE_FRAME_MAX (14)
#define HIT_EF_FRAME_COUNT_MAX (14)
#define HIT_EF_ALPHA (120)

// --- SHOT ---
#define SHOT_EF_WIDTH (240)
#define SHOT_EF_HEIGHT (240)
#define SHOT_EF_SPRITE_FRAME_MAX (12)
#define SHOT_EF_FRAME_COUNT_MAX (12)

// --- LIFE/STAR共通 ---
#define LIFE_STAR_EF_WIDTH (150)
#define LIFE_STAR_EF_HEIGHT (150)
#define LIFE_STAR_EF_SPRITE_FRAME_MAX (5)
#define LIFE_STAR_EF_FRAME_COUNT_MAX (30)

// --- WARP ---
#define WARP_EF_WIDTH (320)
#define WARP_EF_HEIGHT (240)
#define WARP_EF_SPRITE_FRAME_MAX (2)
#define WARP_EFFRAME_COUNT_MAX (26)

// --- BOSS CHARGE ---
#define BOSS_CHARGE_EF_WIDTH (240)
#define BOSS_CHARGE_EF_HEIGHT (240)
#define BOSS_CHARGE_EF_SPRITE_FRAME_MAX (10)
#define BOSS_CHARGE_EFFRAME_COUNT_MAX (40)

// --- 爆発エフェクト---
#define EXPLOSION_WIDTH (200)
#define EXPLOSION_HEIGHT (200)
#define EXPLOSION_SPRITE_COL (8)
#define EXPLOSION_ANIME_END (20)

class EffectManager
{
  public:
	EffectManager();
	~EffectManager();
	void start();
	// エフェクトアニメーション再生
	void playEffectAnimation(); 
	// エフェクトアニメーションの設置
	void setEffect(double *setPositionX, double *setPositionY,
	               int effectType); 
	//パラメータの設定
	void setParameter(Effect *effect, const int playFrameTable[],
	                  int effectType);
	// エフェクトクラス
	Effect *mEffects[MAX_BULLET_NUMBER]; 

  private:
	// 各弾のパラメータ
	static constexpr int mEffectWidthList // 弾の横幅サイズの配列
	    [EFFECT_TYPE_MAX] = {
	        POWER_UP_EF_WIDTH, CHARGE_EF_WIDTH,      HIT_EF_WIDTH,
	        SHOT_EF_WIDTH,     LIFE_STAR_EF_WIDTH,   LIFE_STAR_EF_WIDTH,
	        WARP_EF_WIDTH,     BOSS_CHARGE_EF_WIDTH, EXPLOSION_WIDTH};
	static constexpr int mEffectHightList // 弾の縦幅サイズの配列
	    [EFFECT_TYPE_MAX] = {
	        POWER_UP_EF_HEIGHT, CHARGE_EF_HEIGHT,      HIT_EF_HEIGHT,
	        SHOT_EF_HEIGHT,     LIFE_STAR_EF_HEIGHT,   LIFE_STAR_EF_HEIGHT,
	        WARP_EF_HEIGHT,     BOSS_CHARGE_EF_HEIGHT, EXPLOSION_HEIGHT};
	static constexpr int mEffectSpriteFrameMaxList // 弾の速度の配列
	    [EFFECT_TYPE_MAX] = {
	        POWER_UP_EF_SPRITE_FRAME_MAX,  CHARGE_EF_SPRITE_FRAME_MAX,
	        HIT_EF_SPRITE_FRAME_MAX,       SHOT_EF_SPRITE_FRAME_MAX,
	        LIFE_STAR_EF_SPRITE_FRAME_MAX, LIFE_STAR_EF_SPRITE_FRAME_MAX,
	        WARP_EF_SPRITE_FRAME_MAX,      BOSS_CHARGE_EF_SPRITE_FRAME_MAX,
	        EXPLOSION_SPRITE_COL};
	static constexpr int mEffectPlayFrameMaxList // 弾の速度の配列
	    [EFFECT_TYPE_MAX] = {POWER_UP_EFFECT_FRAME_COUNT_MAX,
	                         CHARGE_EF_FRAME_COUNT_MAX,
	                         HIT_EF_FRAME_COUNT_MAX,
	                         SHOT_EF_FRAME_COUNT_MAX,
	                         LIFE_STAR_EF_FRAME_COUNT_MAX,
	                         LIFE_STAR_EF_FRAME_COUNT_MAX,
	                         WARP_EFFRAME_COUNT_MAX,
	                         BOSS_CHARGE_EFFRAME_COUNT_MAX,
	                         EXPLOSION_ANIME_END};

	//---各エフェクトの再生するフレーム番号を順番に並べたテーブル配列---
	static constexpr int mPowerUpEffectTable
	    [POWER_UP_EFFECT_FRAME_COUNT_MAX] = // パワーアップエフェクトの再生順テーブル
	    {0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7};

	static constexpr int mChargeEffectTable
	    [MAX_TEBLE_FRAME] = // チャージエフェクトの再生順テーブル
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

	//----------------------------------
};
#endif // !__EFFECTMANAGER_H__
