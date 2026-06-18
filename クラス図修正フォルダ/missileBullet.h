#pragma once
#include "bullet.h"

// 角度変換
#define DEGREE_HALF_TURN               (180)
// 描画位置補正
#define MISSILE_DRAW_OFFSET_Y          (10)
//ミサイル弾速度
#define MISAAILE_BULLET_SPD            (10.0f)

class MissileBullet : public Bullet
{
public:
    MissileBullet();
    ~MissileBullet();
    void action();
    void draw();
    void start();
    bool setBullet(int setPositionX, int setPositionY, int setAngle);//ミサイル弾の設置


private:

};

