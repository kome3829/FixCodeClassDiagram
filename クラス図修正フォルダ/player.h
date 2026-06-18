#pragma once
#include "DxLib.h"
#include"myLib/MyLib.h"
#include"myLib/key.h"
#include "math.h"
#include "define.h"
#include "deta.h"
#include"enemybullet.h"
#include"enemyMiniBullet.h"
#include"object.h"
//ヒットポイント
#define MAX_HP (3)
// ダメージ演出
#define PLAYER_DAMAGE_RESET_FRAME      (40)
// 無敵時間ゲージ
#define PLAYER_UNBEATABLE_MAX_FRAME   (180.0)
#define PLAYER_UNBEATABLE_MAX_PERCENT   (1.0)
//表示
#define UNBEATABLE_FLASH_START_FRAME     (120)
#define PLAYER_UNBEATABLE_FLASH_INTERVAL_FRAME  (10)
#define PLAYER_UNBEATABLE_GAUGE_LEFT_OFFSET     (32)
#define PLAYER_UNBEATABLE_GAUGE_TOP_OFFSET      (25)
#define PLAYER_UNBEATABLE_GAUGE_WIDTH           (64)
#define PLAYER_UNBEATABLE_GAUGE_HEIGHT          (3)

#define PLAYER_LIFE_ICON_AMOUNT                (3)
#define PLAYER_LIFE_ICON_BASE_X_OFFSET         (40)   // 20*2
#define PLAYER_LIFE_ICON_SPACING_X             (25)
#define PLAYER_LIFE_ICON_INDEX_MAX             (3)
#define PLAYER_LIFE_ICON_Y_OFFSET              (30)
//当たり判定
#define PLAYER_HIT_RANGE   (10)
//ショットパワー加算量
#define SHOT_POWER_INCREASE_AMONT  (30)
//無敵時間表示バーのカラーコード
#define ORANGE_COLLAR  (0xffff00)

class Player
{
public:
	Player();
	~Player();
	void action();
	void draw();
	void start();
	bool checkEnemyBulletHit(EnemyBullet* enemyBullet, EnemyMiniBullet* enemyMiniBullet);	//敵弾の当たり判定
	int checkItemObjectHit(Object* itemObject, int* score);							//アイテムオブジェクトの当たり判定

	double mX;							//x座標
	double mY;							//y座標
	int mWidth;							//画像の横サイズ
	int mHeight;						//画像の縦サイズ
	bool mIsActiveMissileShot;			//ミサイル弾有効化判定
	double mSpeed;						//移動速度
	int mLife;							//残機
	bool mIsActiveLifeIcon[MAX_LIFE];	//HPアイコン表示判定
	bool mIsDefeat;						//撃破判定
	bool mIsTakeDamage;					//ダメージ判定
	int mDamageDisplayCount;			//ダメージ表示カウント
	int mShotPower;						//ショットパワー
	bool mIsUnbeatable;					//無敵判定
	int mUnbeatableCount;				//無敵時間カウント
	double mUnbeatableGagePercent;	    //無敵時間ゲージの表示範囲割合


private:

};