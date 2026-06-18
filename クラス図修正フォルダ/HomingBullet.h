#pragma once
#include "bullet.h"

#define HOMING_BULLET_SPEED  (8.0f)
#define HOMING_DRAW_OFFSET_Y (10)

class HomingBullet : public Bullet
{
public:
    HomingBullet();
    ~HomingBullet();
    void action();
    void draw();
    void start();
    bool  setBullet(int setPositionX, int setPositionY, int setAngle);//ホーミング弾の設置


private:

};

