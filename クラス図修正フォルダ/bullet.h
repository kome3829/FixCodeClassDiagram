#pragma once
#include "DxLib.h"
#include"myLib/MyLib.h"
#include"myLib/key.h"
#include "math.h"
#include "define.h"
#include "deta.h"

//表示のオフセット
#define DRAW_OFFSET_X (19)
#define DRAW_OFFSET_Y (10)

class Bullet
{
public:
	Bullet();
	~Bullet();
	virtual void action();
	virtual void draw();
	virtual void start();
	virtual bool setBullet(int setPositionX, int setPositionY, int setAngle);	//弾の設置

	double mX;			//X座標
	double mY;			//Y座標
	double mVectorX;	//Xベクトル
	double mVectorY;	//Yベクトル
	double mAngle;		//発射角度
	bool mIsActive;		//有効化判定


private:

};

