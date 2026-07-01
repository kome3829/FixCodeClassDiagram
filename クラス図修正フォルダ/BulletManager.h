#ifndef __BULLETMANAGER_H__
#define __BULLETMANAGER_H__

#include "Bullet.h"
#include "define.h"

// 前方宣言。ヘッダファイルでのインクルードを減らすため
class BossEnemy;
class MinionEnemyManager;
class Player;
class Enemy;

struct BulletData
{
	int width;//横サイズ
	int height;//縦サイズ
	int speed;//速さ
};
// 各弾のパラメータ
constexpr BulletData BULLET_DATA[] = {
    {64, 64, 10}, // プレイヤー通常弾
    {64, 64, 10},    // ミサイル弾
    {20, 20, 8}, // スペシャル弾
    {30, 30, 6}, // 敵通常弾
    {20, 20, 4}, // 敵ミニ弾弾
    {20, 20, 4}  // 敵ミニ弾弾

};
class BulletManager
{
  public:
	BulletManager();
	~BulletManager();
	void action();
	void draw();
	void start();
	void checkHit(
	    Player *player_p, BossEnemy *bossEnemy_p,
	    MinionEnemyManager *minionEnemyManager_p); // 当たり判定処理の管理関数

	void setBullet(int setPositionX, int setPositionY, int setAngle,
	               BULLET_TYPE bulletType, bool isBossEnemy,
	               bool isPlayerBullet); // 設置処理関数

  private:
	Bullet *mBullets[MAX_BULLET_NUMBER]; // 弾クラス

	void checkHitMinionEnemy(MinionEnemyManager *minionEnemyManager,
	                         Bullet *bullet); // 各雑魚敵との当たり判定処理関数
	void checkHitEnemy(Enemy *enemy, Bullet *bullet); // 敵との判定処理


};

#endif // __BULLETMANAGER_H__