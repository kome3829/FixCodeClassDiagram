#pragma once
#include "enemy.h"
// --- 円運動開始 ---
#define ARC_MOVE_START_FRAME        (331)
// --- チャージ後無敵 ---
#define UNBEATABLE_START_FRAME      (240)
#define UNBEATABLE_END_FRAME        (280)
// --- ワープ ---
#define WARP_START_FRAME            (280)
#define WARP_END_FRAME              (330)
#define WARP_TRIGGER_FRAME          (304)

class ChargeEnemy : public Enemy
{
public:
	ChargeEnemy();
	~ChargeEnemy();
	 void action();
	 void draw();
	 void start();


private:

};

