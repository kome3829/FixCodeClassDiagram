#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "DxLib.h"
#include "define.h"
#include "deta.h"
#include "math.h"
#include "myLib/MyLib.h"
#include "myLib/key.h"
#include "object.h"
#include "Character.h"
#include "EffectManager.h"
// ヒットポイント
#define MAX_HP (3)
// ダメージ演出
#define PLAYER_DAMAGE_RESET_FRAME (40)
// 無敵時間ゲージ
#define PLAYER_UNBEATABLE_MAX_FRAME (180.0)
#define PLAYER_UNBEATABLE_MAX_PERCENT (1.0)
// 表示
#define UNBEATABLE_FLASH_START_FRAME (120)
#define PLAYER_UNBEATABLE_FLASH_INTERVAL_FRAME (10)
#define PLAYER_UNBEATABLE_GAUGE_LEFT_OFFSET (32)
#define PLAYER_UNBEATABLE_GAUGE_TOP_OFFSET (25)
#define PLAYER_UNBEATABLE_GAUGE_WIDTH (64)
#define PLAYER_UNBEATABLE_GAUGE_HEIGHT (3)

#define PLAYER_LIFE_ICON_AMOUNT (3)
#define PLAYER_LIFE_ICON_BASE_X_OFFSET (40) // 20*2
#define PLAYER_LIFE_ICON_SPACING_X (25)
#define PLAYER_LIFE_ICON_INDEX_MAX (3)
#define PLAYER_LIFE_ICON_Y_OFFSET (30)
// 当たり判定
#define PLAYER_HIT_RANGE (10)
// ショットパワー加算量
#define SHOT_POWER_INCREASE_AMONT (30)
// 無敵時間表示バーのカラーコード
#define ORANGE_COLLAR (0xffff00)

// 発射管理
#define PLAYER_NOMAL_SHOT_INTERVAL (5)

// 通常弾
#define PLAYER_NORMAL_SHOT_SPEED (2)
#define PLAYER_UPGRADE_SHOT_SPEED (1)
#define PLAYER_NORMAL_SHOT_ANGLE (-90)

// ミサイル弾
#define PLAYER_MISSILE_SHOT_INTERVAL (3) // 変数に代入から直接記述に変更
#define PLAYER_MISSILE_AMOUNT (2)
#define PLAYER_MISSILE_START_ANGLE (70)
#define PLAYER_MISSILE_ANGLE_STEP (40)
#define PLAYER_MISSILE_SHOT_COUNTER_RESET (1024)
#define TURN_AROUND_ANGLE (180)

// スペシャル弾
#define PLAYER_SPECIAL_AMOUNT (5)
#define PLAYER_SPECIAL_SHOT_ACTIVE_COUNT (5)
#define PLAYER_SPECIAL_SHOT_COUNTER_RESET (10)

#define SPECIAL_BULLET_ANGLE_STEP (30)
#define SPECIAL_BULLET_BASE_ANGLE (-90)
#define SPECIAL_BULLET_SPREAD_COUNT (4)

// 発射位置ののオフセット
#define FIRE_POINT_OFFSET_X (18)
#define FIRE_POINT_OFFSET_Y (-5)


class BulletManager;

class Player : public Character
{
  public:
	Player();
	~Player();
	void action(BulletManager *bulletManager, EffectManager *effectManager,
	            int *score);
	void draw();
	void start();
	int mImageWidth;  // 画像の横サイズ
	int mImageHeight; // 画像の縦サイズ

	bool mIsDamegeCoolDown;//ダメージ処理の無効化
	bool mIsItemHit;//アイテムヒット判定
	int mHitItemType;//当たったアイテムの種類

	bool mIsActiveMissileShot;        // ミサイル弾有効化判定
	double mSpeed;                    // 移動速度
	int mLife;                        // 残機
	bool mIsActiveLifeIcon[MAX_LIFE]; // HPアイコン表示判定
	bool mIsDefeat;                   // 撃破判定
	int mDamageDisplayCount;          // ダメージ表示カウント
	int mShotPower;                   // ショットパワー
	bool mIsUnbeatable;               // 無敵判定
	int mUnbeatableCount;             // 無敵時間カウント
	double mUnbeatableGagePercent; // 無敵時間ゲージの表示範囲割合

		//---プレイヤーの発射に関する変数---
	int mShotCount;          // プレイヤー発射カウント
	int mNomalShotIntervalCount;   // 通常弾発射間隔カウント
	int mNomalShotInterval;        // 通常弾発射間隔
	int mMissileShotIntervalCount; // ミサイル発射間隔カウント
	int mSpecialShotCount;         // スペシャル弾発射カウント
  private:
	/// <summary>
	/// プレイヤー弾発射関数
	/// </summary>
	void shotPlayerBullet(BulletManager *bulletManager,
	                      EffectManager *effectManager);
	// ダメージ処理
	void takeDamage();
	//アイテム効果反映処理
	void applyItemeffect(int *score, EffectManager *effectManager);
};

#endif // !__PLAYER_H__

