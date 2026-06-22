#ifndef __BULLETMANAGER_H__
#define __BULLETMANAGER_H__

#include "DxLib.h"
#include "MinionEnemyManager.h"
#include "bossEnemy.h"
#include "bullet.h"
#include "define.h"
#include "deta.h"
#include "math.h"
#include "player.h"




class BulletManager {
public:
  BulletManager();
  ~BulletManager();
   void action();
   void draw();
   void start();
   void checkHit(Player *player_p, BossEnemy *bossEnemy_p,
                 MinionEnemyManager *minionEnemyManager_p);
   void setBullet(int setPositionX, int setPositionY, int setAngle,
                  BULLET_TYPE bulletType);
  Bullet* mBullets[MAX_BULLET_NUMBER]; // 弾クラス


private:
    
};

#endif // __BULLETMANAGER_H__