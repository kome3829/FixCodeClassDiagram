#pragma once
#include"define.h"
#include"DxLib.h"
#include"deta.h"
// --- 爆発エフェクトサイズ ---
#define EXPLOSION_WIDTH   (200)
#define EXPLOSION_HEIGHT  (200)
// --- 爆発アニメ終了フレーム ---
#define EXPLOSION_ANIME_END    (20)
// --- スプライト分割数（横8枚）
#define EXPLOSION_SPRITE_COL   (8)
class Explosion
{
public:
	Explosion();
	~Explosion();
	void start();
	void playExplosionAnimation();		//爆発エフェクトアニメーションの再生
	bool setExplosion(int setPositionX, int setPositionY);	//爆発エフェクトアニメーションの設置

	int mX;							//X座標
	int mY;							//Y座標
	int mCurrentPlayFrameNumber;	//現在の再生フレーム番号
	int mSpriteFrameX;				//スプライトシート上の横方向のコマ番号
	int mSpriteFrameY;				//スプライトシート上の縦方向のコマ番号
	int mFrameCount;				//爆発アニメーション再生カウント
	int mWidth;						//画像横サイズ
	int mHeight;					//画像縦サイズ
	bool mIsPlay;					//再生判定


private:
	static constexpr int mExplosionPlayFrameTable[EXPLOSION_ANIME_END] =//爆発エフェクトの再生順テーブル
	{
		0,0,0,1,1,1,2,2,3,3,
		4,4,5,5,6,6,6,7,7,7
	};

};