#ifndef __BULLETMANAGER_H__
#define __BULLETMANAGER_H__

#include "DxLib.h"
#include "bossEnemy.h"
#include "Bullet.h"
#include "define.h"
#include "deta.h"
#include "math.h"
#include "player.h"
#include "Character.h"

class MinionEnemyManager;

class BulletManager {
public:
  BulletManager();
  ~BulletManager();
   void action();
   void draw();
   void start();
   void checHit(Player *player_p, BossEnemy *bossEnemy_p,
                 MinionEnemyManager *minionEnemyManager_p);

   void setBullet(int setPositionX, int setPositionY, int setAngle,
	              BULLET_TYPE bulletType, bool isBossEnemy);
  Bullet* mBullets[MAX_BULLET_NUMBER]; // 弾クラス


private:
  void checkHitMinionEnemyPre(  MinionEnemyManager *minionEnemyManager,
	                     Bullet *bullet);

  //各弾のパラメータ
  static constexpr int mBulletWidthList//弾の横幅サイズの配列
	  [MAX_BULLET_TYPE_NUMBER] = 
	  {64, 64, 20, 30, 20, 20};
  static constexpr int mBulletHightList // 弾の縦幅サイズの配列
	  [MAX_BULLET_TYPE_NUMBER] = {48, 48, 20, 30, 20, 20};
  static constexpr int mBulletSpeedList // 弾の速度の配列
	  [MAX_BULLET_TYPE_NUMBER] = {10, 10, 8, 6, 4, 4};
};

#endif // __BULLETMANAGER_H__