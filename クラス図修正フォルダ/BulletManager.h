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
	int width;  // 横サイズ
	int height; // 縦サイズ
	int speed;  // 速さ
};
// 各弾のパラメータ
constexpr BulletData BULLET_DATA[] = {
    {PLAYER_BULLET_WIDTH, PLAYER_BULLET_HEIGHT,
     PLAYER_BULLET_SPEED}, // プレイヤー通常弾
    {PLAYER_MISSILE_BULLET_WIDTH, PLAYER_MISSILE_BULLET_HEIGHT,
     PLAYER_MISSILE_BULLET_SPEED}, // ミサイル弾
    {PLAYER_SPECIAL_BULLET_WIDTH, PLAYER_SPECIAL_BULLET_HEIGHT,
     PLAYER_SPECIAL_BULLET_SPEED}, // スペシャル弾
    {ENEMY_BULLET_WIDTH, ENEMY_BULLET_HEIGHT, ENEMY_BULLET_SPEED}, // 敵通常弾
    {ENEMY_BULLET_MINI_WIDTH, ENEMY_BULLET_MINI_HEIGHT,
     ENEMY_BULLET_MINI_SPEED}, // 敵ミニ弾弾(黄色)
    {ENEMY_BULLET_MINI_WIDTH, ENEMY_BULLET_MINI_HEIGHT,
     ENEMY_BULLET_MINI_SPEED} // 敵ミニ弾弾(オレンジ)

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