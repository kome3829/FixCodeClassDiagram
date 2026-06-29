#ifndef __TRACEENEMY_H
#define __TRACEENEMY_H

#include "Enemy.h"
// 追従速度
#define TRACE_ENEMY_TRACK_SPEED (4.0)
// --- 追尾敵弾発射タイミング ---
#define TRACE_ENEMY_SHOT_START_FRAME (60)
#define TRACE_ENEMY_SHOT_INTERVAL_FRAME (10)
#define TRACE_ENEMY_SHOT_END_FRAME (100)
#define TRACE_ENEMY_SHOT_RESET_FRAME (180)
// --- 4方向弾 ---
#define TRACE_ENEMY_SHOT_WAY_COUNT (4)
#define TRACE_ENEMY_SHOT_BASE_ANGLE (90)

class TraceEnemy : public Enemy
{
  public:
	TraceEnemy();
	~TraceEnemy();
	void action(int playerX, int playerY, int *score,
	            BulletManager *bulletManager, EffectManager *effectManager,
	            ItemObjectManager *itemObject, Player *player);
	void draw();
	void start();	
	// 追従敵弾発射関数
	void shotTraceEnemyBullet(BulletManager *bulletManager,
	                          EffectManager *effectManager);
  private:
};
#endif // !__TRACEENEMY_H
