#pragma once
#include "enemy.h"
//追従速度
#define TRACE_ENEMY_TRACK_SPEED              (4.0)

class TraceEnemy : public Enemy
{
public:
    TraceEnemy();
    ~TraceEnemy();
    void action(int playerX, int playerY);
    void draw();
    void start();


private:

};

