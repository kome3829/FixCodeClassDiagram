#pragma once
#include "bullet.h"

// --- 敵ミニ弾速度 ---
#define ENEMY_MINI_BULLET_FAST_SPEED        (4.0f)

class EnemyMiniBullet : public Bullet
{
public:
    EnemyMiniBullet();
    ~EnemyMiniBullet();
    void draw();
    void start();
    bool setBullet(int setPositionX, int setPositionY, double setAngle, int imageType);//敵ミニ弾の設置

    int mImageType;//表示画像種類(色違い)


private:

};

