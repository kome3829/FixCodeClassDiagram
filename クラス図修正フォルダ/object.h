#pragma once
#include"math.h"
#include"define.h"
#include "DxLib.h"
#include"Deta.h"

// EXPアイテム
#define OBJECT_EXP_FAST_SPEED       (8.0)
#define OBJECT_EXP_HOMING_START_FRAME      (30)
#define OBJECT_EXP_SPEEDDOWN         (0.9f)
// LIFE / STAR ホーミング
#define OBJECT_ITEM_HOMING_SPEED        (10.0f)

// 画面外判定余白
#define OBJECT_OUTSIDE_MARGIN             (32)
class Object
{
public:
	Object();
	~Object();
	void action();
	void draw();
	void start();
	void reset();//リセット関数
	void judgeHomingMoveEneble();	//吸い付き有効化判定

	double mX;					//X座標
	double mY;					//Y座標
	double* mPlayerX;			//プレイヤーのX座標。ポインタの理由は常に更新されれている変数のため直接参照する必要がある。
	double* mPlayerY;			//プレイヤーのY座標。ポインタの理由は常に更新されれている変数のため直接参照する必要がある。
	int mItemObjectType;			//オブジェクトの種類
	double mVectorX;			//Xベクトル
	double mVectorY;			//Yベクトル
	int mMovementAngle;			//移動用角度
	int mEXPMoveCount;			//経験値用移動カウント
	bool mIsActive;				//有効判定
	bool mIsActiveHomingMove;	//吸い付き判定


private:

};