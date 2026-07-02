#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "Deta.h"
#include "define.h"

// 最大フレーム数
#define MAX_TABLE_FRAME (120)

class Effect
{
  public:
	Effect();
	~Effect();
	void start();
	void playEffectAnimation(); // エフェクトアニメーション再生

	double
	    *mX; // X座標ポインタ。位置変数が常に更新されるため。ポインタで直接参照
	double
	    *mY; // Y座標ポインタ。位置変数が常に更新されるため。ポインタで直接参照
	int mCurrentPlayFrameNumber; // 現在の再生フレーム番号
	int mSpriteFrameX; // スプライトシート上の横方向のコマ番号
	int mSpriteFrameY; // スプライトシート上の縦方向のコマ番号
	int mFrameCount;   // 再生フレームカウント
	int mWidth;        // 画像横サイズ
	int mHeight;       // 画像縦サイズ
	int mEffectType;   // エフェクトの種類
	int mSpriteFrameMax; // スプライト画像の分割数
	int mPlayFrameMax;   // 最大再生フレーム
	int mAlpha;          // 透過度
	bool mIsPlay;        // 再生判定

	int mPlayFrameTable
	    [MAX_TABLE_FRAME]; // エフェクトアニメーションのフレーム再生順のテーブル
  private:
};

#endif // !__EFFECT_H__
