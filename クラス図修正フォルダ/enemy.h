#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Character.h"
#include "Deta.h"
#include "DxLib.h"
#include "EffectManager.h"
#include "ItemObjectManager.h"
#include "Player.h"
#include "define.h"
#include "math.h"
#include "myLib/MyLib.h"
#include "myLib/key.h"
class BulletManager; // 関数の引数利用のための前提宣言

// action関数の引数を短くするため作成。必要なクラスポインタの構造体
struct EnemyActionData
{
	Player *mPlayer;
	int *mScore;
	BulletManager *mBulletManager;
	EffectManager *mEffectManager;
	ItemObjectManager *mItemObjectManager;
};


// --- 移動フレーム制御 ---
#define MOVE_INTERVAL_FRAME (2)
// --- 出現位置限界 ---
#define ENEMY_DESCEND_LIMIT_Y (100)
// --- 位置到達判定 ---
#define ENEMY_POSITION_DIFFERENCE_RANGE (6)
// --- ポジション移動速度 ---
#define ENEMY_POSITION_MOVE_SPEED (5.0f)
// --- 退場フレーム ---
#define ENEMY_EXIT_START_FRAME (250)
// --- 横移動速度上限 ---
#define ENEMY_SPEEDUP_LIMIT_X (3.5f)

// --- MoveCount上限（オーバーフロー対策） ---
#define MOVE_COUNT_LIMIT (2147483646)
// ダメージ表示カウント
#define DAMAGE_COUNT_LIMIT (2)
// ボスステージ設定
#define BOSS_STAGE_START_COUNT (10)

// --- 当たり判定マージン ---
#define HITBOX_MARGIN_X (4)
#define HITBOX_MARGIN_Y (10)

class Enemy : public Character
{
  public:
	Enemy();
	~Enemy();
	virtual void action(EnemyActionData *actionData);
	void draw();
	void start();
	void pop(int popPositionX, int popPositionY, int number,
	         bool isBossStage); // 出現処理
	void playSoundEffect();     // 死亡SEの再生

	int mNumber;     // 識別用番号
	double mVectorX; // Xベクトル
	double mVectorY; // Yベクトル
	int mHitPoint;   // 体力

	bool mIsDamageCoolDown;       // ダメージ無効判定
	int mMoveCount;               // 移動カウント
	bool mIsDefeat;               // 撃破判定
	int mShotCount;               // 発射カウント
	int mDamageDisplayCount;      // ダメージ表示カウント
	bool mIsReachedStartPosition; // 追従敵の登場移動判定
	bool mIsBossStage;            // ボスステージ判定
	bool mIsReachedBossPosition;  // ボスステージの移動位置についたか判定
	bool mIsInvincible;           // 無敵判定　ボス戦のチャージ敵のみ使用
	int mAlpha;                   // 画像のα値
	double mArcMotionAngle;       // 円運動　角度
	double mArcMotionRadius;      // 円運動　半径

	bool mIsWarp;	//ワープ最中か判定

	// ダメージ処理
	void takeDamage(int *score, EffectManager *effectManager,
	                ItemObjectManager *itemObjectManager, Player *player);
	// 通常敵弾発射関数
	void shotEnemyBullet(BulletManager *bulletManager,
	                     EffectManager *effectManager);

  private:
};
#endif // !__ENEMY_H__
