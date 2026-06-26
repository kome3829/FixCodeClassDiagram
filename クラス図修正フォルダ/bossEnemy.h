#ifndef __BOSSENEMY_H__
#define __BOSSENEMY_H__

#include "MinionEnemyManager.h"
#include "enemy.h"
// HP
#define BOSS_ENEMY_MAX_HP_PERCENT (100)
#define BOSS_HP_BER_MAXWIDTH (315)
// アルファ値
#define ALPHA_MAX (255)
// 当たり判定余白
#define HIT_RANGE_MARGIN_X (4)
#define HIT_RANGE_MARGIN_Y (10)

// 切り替え行動数
#define BOSS_PATTARN_MAX (3)
// 移動
#define BOSS_DESCEND_LIMIT_Y (40)
#define BOSS_MOVE_INTERVAL_FRAME (15)
#define POSITION_TOLERANCE (3)
// スピードダウンの下限
#define BOSS_ENEMY_SPD_DOWN_LIMIT (1.5f)

#define START_MOVE_DELAY_FRAME (30) // 移動ディレイカウント
#define START_MOVE_DAMAGE (30)      // 行動切り替えに必要なダメージ量

// warpMoveFourPoint関数内で使用
#define MOVE_PHASE1_END (75)  // 左移動終了
#define MOVE_PHASE2_END (225) // 右移動終了
#define MOVE_PHASE3_END (300) // 再度左移動終了
#define MOVE_PHASE4_END (360) // ワープ前処理終了
#define MOVE_END_FRAME (361)  // 終了フレーム
// --- 攻撃停止 ---
#define SHOT_STOP_INTERVAL (40)
// --- ワープ演出 ---
#define WARP_TRIGGER_FRAME_BOSS (334)

// warpMoveAndChargeAttackMove関数内で使用

// --- フェーズ境界フレーム ---
#define SUMMON_ENEMY_SET_FRAME (10)
#define WAIT1_END_FRAME (280)
#define WARP1_END_FRAME (330)
#define WARP1_RESET_FRAME (331)

#define WARP2_START_FRAME (600)
#define WARP2_END_FRAME (700)
#define WARP2_RESET_FRAME (701)

#define WAIT2_END_FRAME (742)
#define MOVE_RIGHT1_END_FRAME (822)
#define MOVE_LEFT_END_FRAME (902)
#define MOVE_RIGHT2_END_FRAME (982)
#define LOOP_RESET_FRAME (983)

// --- チャージ関連 ---
#define CHARGE_EFFECT_INTERVAL (40)
#define CHARGE_END_FRAME_BOSS (240)

// --- ワープ演出 ---
#define WARP1_TRIGGER_FRAME (304)
#define WARP2_ALPHA_RESET_FRAME (649)
#define WARP2_DAMAGE_START_FRAME (650)
#define WARP2_TRIGGER_FRAME (674)

// --- ボス弾幕 ---

// ミニ弾 共通
#define BOSS_MINI_SHOT_INTERVAL_FRAME (30)
#define BOSS_MINI_SHOT_AMOUNT (12)
#define BOSS_MINI_SHOT_ANGLE_STEP (30)

// 連射ミニ弾
#define BOSS_MINI_BURST_RESET_FRAME (30)
#define BOSS_MINI_BURST_ACTIVE_FRAME (15)
#define BOSS_MINI_BURST_INTERVAL_FRAME (4)
#define BOSS_MINI_BURST_OFFSET_ANGLE (20)

// 通常弾
#define BOSS_NOMAL_SHOT_INTERVAL_FRAME (120)
#define BOSS_NOMAL_SHOT_AMOUNT (30)
#define BOSS_NOMAL_SHOT_ANGLE_STEP (12)

// 爆発演出
#define BOSS_EXPLOSION_COUNT (4)
#define BOSS_EXPLOSION_START_X (-30)
#define BOSS_EXPLOSION_STEP_X (40)
#define BOSS_EXPLOSION_OFFSET_Y (40)

// 雑魚召喚位置
#define SUMMON_LEFT_CHARGE_ENEMY_X (30)
#define SUMMON_LEFT_NOMAL_ENEMY_X (10)
#define SUMMON_LEFT_TRACE_ENEMY_X (50)

#define SUMMON_RIGHT_CHARGE_ENEMY_X (610)
#define SUMMON_RIGHT_NOMAL_ENEMY_X (580)
#define SUMMON_RIGHT_TRACE_ENEMY_X (630)

#define SUMMON_ENEMY_Y (0)

#define SUMMON_ENEMY_Y (0)

enum SHOT_PATTERN
{
	FAST_EXECUTE,
	SECOND_EXECUTE,
	THIRD_EXECUTE,
};
enum BOSS_DIRECTION
{
	LEFT,
	RIGHT,
	POINT_LEFT,
	POINT_BOTTOM,
	POINT_RIGHT,
	POINT_TOP,
};

class BossEnemy : public Enemy
{
  public:
	BossEnemy();
	~BossEnemy();
	void action(int *score, BulletManager *bulletManager,
	            EffectManager *effectManager,
	            ItemObjectManager *itemObjectManager,
	            MinionEnemyManager *minionEnemyManager, Player *player);
	void draw();
	void start();
	bool moveCenterPosition();  // 中央位置に配置
	void moveLeftAndRight();    // 左右移動
	void patrolMoveFourPoint(); // 4つのポイントを巡回移動
	void warpMoveFourPoint(
	    EffectManager *effectManager); // 4つのポイントをワープ移動
	void warpAndAttackMove(
	    EffectManager
	        *effectManager); // ワープ移動とチャージ攻撃移動の組み合わせた移動
	void moveDefeat();       // 撃破時の移動
	void settingNextPatrolMove();        // HP75％時の次の移動の準備
	void settingNextWarpMove();          // HP50％時の次の移動の準備
	void settingNextWarpAndAttackMove(); // HP50％時の次の移動の準備

	unsigned int mBossHitPointPercent; // HPパーセント
	bool
	    mIsExecuteProcess[BOSS_PATTARN_MAX]; // パターン切り替え時処理の実行判定
	bool mIsDropExperiencePoint;             // ボスが経験値を落としたか
	int mShotBulletCount;                    // 弾発射カウント
	int mShotMiniBulletCount;                // ミニ弾発射カウント
	int mAlpha;                              // 透過度
	double mSpeed;                           // 移動速度
	int mBossHitPointBarRightEnd;            // ボスHPバーの右端座標
	double mCentralPositionAngle;            // 中央位置の角度
	double mCenterPositionVectorX;           // 中央までのXベクトル
	double mCenterPositionVectorY;           // 中央までのYベクトル
	double mMovementVectorX;                 // 指定位置までの移動Xベクトル
	double mMovementVectorY;                 // 指定位置までの移動Yベクトル
	double mMoveTargetAngle;                 // 指定位置までの角度

	bool mHasSetSpeed;      // 移動速度を設定したか判定
	bool mIsUnbeatable;     // 無敵判定　　　
	bool mIsResetMoveCount; // 移動カウントのリセット判定
	bool mIsSummonEnemy;    // 雑魚敵召喚判定

  private:
	// ダメージ処理
	void takeDamage(int *score);
	/// <summary>
	/// ボス敵弾発射関数
	/// </summary>
	void shotBossEnemyBullet(BulletManager *bulletManager,
	                         EffectManager *effectManager,
	                         ItemObjectManager *itemObjectManager,
	                         MinionEnemyManager *minionEnemyManager,
	                         Player *player);

	BOSS_DIRECTION mBossDirection; // ボス向いてる方向
};
#endif // !__BOSSENEMY_H__
