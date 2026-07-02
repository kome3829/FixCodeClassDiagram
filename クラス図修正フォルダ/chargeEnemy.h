#ifndef __CHARGEENEMY_H__
#define __CHARGEENEMY_H__

#include "Enemy.h"
// --- 円運動開始 ---
#define ARC_MOVE_START_FRAME        (331)
// --- チャージ後無敵 ---
#define UNBEATABLE_START_FRAME      (240)
#define UNBEATABLE_END_FRAME        (280)
// --- ワープ ---
#define WARP_START_FRAME            (280)
#define WARP_END_FRAME              (330)
#define WARP_TRIGGER_FRAME          (304)
// --- ボスステージ用 MoveCount ---
#define CHARGE_ENEMY_BOSS_CHARGE_END (240)
#define CHARGE_ENEMY_BOSS_WARP_EF_FRAME (304)
#define CHARGE_ENEMY_BOSS_ATTACK_START (331)
#define CHARGE_ENEMY_BOSS_ITEM_DROP_FRAME (345)
// --- チャージ敵 射撃タイミング ---
#define CHARGE_EF_INTERVAL (40)
#define CHARGE_EF_END (120)
#define CHARGE_ENEMY_SHOT_START_FRAME (120)
#define CHARGE_ENEMY_SHOT_INTERVAL_FRAME (10)
#define CHARGE_ENEMY_SHOT_RESET_FRAME (170)
// --- ボスステージ射撃 ---
#define CHARGE_ENEMY_RING_SHOT_INTERVAL (50)
#define CHARGE_ENEMY_RING_BULLET_AMOUNT (10)
#define CHARGE_ENEMY_RING_SHOT_ANGLE (36)
#define CHARGE_ENEMY_ANGLE_STEP (20)


#define CHARGE_ENEMY_AIM_SHOT_START (80)
#define CHARGE_ENEMY_AIM_SHOT_INTERVAL (5)
#define CHARGE_ENEMY_AIM_BULLET_AMOUNT (3)

// --- 救済アイテム出現位置 ---

#define ITEM_DROP_LEFT_X (100)
#define ITEM_DROP_RIGHT_X (500)
#define ITEM_DROP_Y (240)

class ChargeEnemy : public Enemy
{
public:
	ChargeEnemy();
	~ChargeEnemy();
	void action(EnemyActionData *actiondata);
	 void draw();
	 void start();

	 	/// <summary>
	 /// チャージ敵弾発射関数
	 /// </summary>
	 /// <param name="chargeEnemy=ChargeEnemyクラスのポインタ"></param>
	 void shotChargeEnemyBullet(BulletManager *bulletManager, Player *player,
	                            EffectManager *effectManager, ItemObjectManager *itemObjectManager);

   private:

};
#endif // !__CHARGEENEMY_H__
