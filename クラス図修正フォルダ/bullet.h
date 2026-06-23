#ifndef __BULLET_H__
#define __BULLET_H__

#include "Character.h"
#include "DxLib.h"
#include "define.h"
#include "deta.h"
#include "math.h"
#include "myLib/MyLib.h"
#include "myLib/key.h"

// --- 敵弾速度 ---
// --- 敵弾速度 ---
#define ENEMY_BULLET_FAST_SPEED (3.0f)
#define BOSS_BULLET_FAST_SPEED (4.0f)
#define BOSS_BULLET_SPEEDDOWN_LIMIT (2.0f)

// 表示のオフセット
#define DRAW_OFFSET_X (19)
#define DRAW_OFFSET_Y (10)
// 当たり判定
#define PLAYER_HIT_RANGE (10)

enum BULLET_TYPE
{
	PLAYER_NOMAL = 0,
	PLAYER_MISSILE,
	PLAYER_SPECIAL,
	ENEMY_NOMAL,
	ENEMY_MINI_ORANGE,
	ENEMY_MINI_YELLOW,
};

class Bullet
{
  public:
	Bullet();
	~Bullet();
	virtual void action();
	virtual void draw();
	virtual void start();
	virtual bool setBullet(int setPositionX, int setPositionY,
	                       int setAngle); // 弾の設置
	bool hitCheck(Character *class_p);    // 弾の当たり判定
	//現状の当たり判定関数に組み込んでみるための仮関数
	bool hitCheckPre(int x, int y, int w, int h); // 弾の当たり判定
	double mX;                         // X座標
	double mY;               // Y座標
	double mVectorX;         // Xベクトル
	double mVectorY;         // Yベクトル
	int mWidth;              // 画像の横サイズ
	int mHeight;             // 画像の縦サイズ
	BULLET_TYPE mBulletType; // 弾の種別

	double mAngle;  // 発射角度
	bool mIsActive; // 有効化判定
	double mSpeed;  // 敵弾の移動速度
	bool mIsBossEnemy; // ボス敵か判定


  private:
};

#endif // __BULLET_H__
