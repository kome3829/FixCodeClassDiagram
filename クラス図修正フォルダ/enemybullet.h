#pragma once
#include "bullet.h"

// --- 敵弾速度 ---
#define ENEMY_BULLET_FAST_SPEED        (3.0f)
#define BOSS_BULLET_FAST_SPEED         (4.0f)
#define BOSS_BULLET_SPEEDDOWN_LIMIT    (2.0f)

class EnemyBullet : public Bullet
{
public:
    EnemyBullet();
    ~EnemyBullet();
    void action();
    void draw();
    void start();
    bool setBullet(int setPositionX, int setPositionY, int setAngle,bool isBossEnemy);//敵弾の設置

    bool mIsBossEnemy;  //ボス敵か判定
    double mSpeed;      //敵弾の移動速度


private:

};

