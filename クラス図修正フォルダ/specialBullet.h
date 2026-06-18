#pragma once
#include "bullet.h"

#define SPECIAL_DRAW_OFFSET_Y          (10)

#define SPECIAL_BULLET_ANGLE_STEP        (30)
#define SPECIAL_BULLET_BASE_ANGLE          (-90)
#define SPECIAL_BULLET_SPREAD_COUNT          (4)
#define SPECIAL_BULLET_SPEED               (8.0)

class SpecialBullet : public Bullet
{
public:
    SpecialBullet();
    ~SpecialBullet();
    void action();
    void draw();
    void start();
    bool setBullet(int setPositionX, int setPositionY, int shotNumber);//スペシャル弾の設置


private:

};

