#pragma once
#include"define.h"
#include"myLib/MyLib.h"
#include"myLib/key.h"
#include "DxLib.h"
#include "deta.h"
#include"bullet.h"
#include"specialBullet.h"
#include"missileBullet.h"
#include"HomingBullet.h"
// --- 移動フレーム制御 ---
#define MOVE_INTERVAL_FRAME    (2)
// --- 出現位置限界 ---
#define ENEMY_DESCEND_LIMIT_Y (100)
// --- 位置到達判定 ---
#define ENEMY_POSITION_DIFFERENCE_RANGE     (6)
// --- ポジション移動速度 ---
#define ENEMY_POSITION_MOVE_SPEED    (5.0f)
// --- 退場フレーム ---
#define ENEMY_EXIT_START_FRAME     (250)
// --- 横移動速度上限 ---
#define ENEMY_SPEEDUP_LIMIT_X            (3.5f)

// --- MoveCount上限（オーバーフロー対策） ---
#define MOVE_COUNT_LIMIT          (2147483646)
//ダメージ表示カウント
#define DAMAGE_COUNT_LIMIT        (2)
//ボスステージ設定
#define BOSS_STAGE_START_COUNT    (10)
// --- 当たり判定マージン ---
#define HITBOX_MARGIN_X (4)
#define HITBOX_MARGIN_Y (10)
class Enemy
{
public:
	Enemy();
	~Enemy();
	virtual void action();
	 void draw();
	 void start();
	 void pop(int popPositionX, int popPositionY, int number,bool isBossStage);//出現処理
	 virtual bool checkPlayerBulletHit(Bullet* bullet, MissileBullet* missileBullet, SpecialBullet* specialBullet, HomingBullet* homingBullet, int* score);//当たり判定
	 void playSoundEffect();//死亡SEの再生

	int mNumber;					//識別用番号
	double mX;						//x座標
	double mY;						//y座標
	double mVectorX;				//Xベクトル
	double mVectorY;				//Yベクトル
	int mWidth;						//画像の横サイズ
	int mHeight;					//画像の縦サイズ
	int mHitPoint;					//体力
	bool mIsActive;					//有効化判定
	int mMoveCount;					//移動カウント
	bool mIsDefeat;					//撃破判定
	int mShotCount;					//発射カウント
	bool mIsTakeDamage;				//ダメージ判定
	int mDamageDisplayCount;		//ダメージ表示カウント
	bool mIsReachedStartPosition;	//追従敵の登場移動判定
	bool mIsBossStage;				//ボスステージ判定
	bool mIsReachedBossPosition;	//ボスステージの移動位置についたか判定
	bool mIsInvincible;				//無敵判定　ボス戦のチャージ敵のみ使用
	int mAlpha;						//画像のα値
	double mArcMotionAngle;			//円運動　角度
	double mArcMotionRadius;		//円運動　半径


private:

};

