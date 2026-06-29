#ifndef __GAMEMAINSCENE_H__
#define __GAMEMAINSCENE_H__

#include "BasicScene.h"
#include "BulletManager.h"
#include "EffectManager.h"
#include "ItemObjectManager.h"
#include "MinionEnemyManager.h"
// --- フェード演出 ---
#define FADE_OUT_SPEED (3)
#define REDBG_FADEOUT_COUNT_MAX (3)

// --- テスト用ボスHP ---
#define TEST_BOSS_HP (1500)

// --- テスト用プレイヤー攻撃力 ---
#define TEST_PLAYER_POWER (6100)

// --- 背景動画ループ ---
#define MOVIE_LOOP_FRAME (600)
#define MOVIE_LOOP_START (0)
// --- UIエリア ---
#define UI_BOX_TOP (460)
#define UI_BOX_BOTTOM (480)
#define UI_SCORE_X (10)
#define UI_SHOT_POWER_X (350)
#define UI_TIME_X (200)

// --- ボスHP表示 ---
#define BOSS_HP_TEXT_X (10)
#define BOSS_HP_TEXT_Y (20)
#define BOSS_HP_BAR_LEFT_X (0)
#define BOSS_HP_BAR_TOP_Y (0)
#define BOSS_HP_BAR_RIGHT_X (320)
#define BOSS_HP_BAR_BOTTOM_Y (14)
#define BOSS_HP_INNER_LEFT_X (2)
#define BOSS_HP_INNER_TOP_Y (2)
#define BOSS_HP_INNER_BOTTOM_Y (12)

// --- フォントサイズ ---
#define UI_FONT_SIZE (16)

// --- 色 ---
#define COLOR_BLACK (0x000000)
#define COLOR_WHITE (0xffffff)
#define COLOR_GREEN (0x00ff00)
#define COLOR_RED (0xff0000)
#define COLOR_RED (0xff0000)

// ボス撃破演出
#define DEFEAT_EXPLOSION_INTERVAL_FRAME (15)

// 爆発位置ランダム
#define BOSS_DEFEAT_EXPLOSION_OFFSET_X (-30)
#define BOSS_DEFEAT_EXPLOSION_OFFSET_Y (-30)
#define BOSS_DEFEAT_EXPLOSION_RANDOM_RANGE (5)
#define BOSS_DEFEAT_EXPLOSION_STEP (20)

// 時間変換
#define TIME_MILLISECOND_TO_SECOND (1000)
#define TIME_SECOND_PER_MINUTE (60)

// プレイヤー死亡後リザルト遷移
#define PLAYER_DEFEAT_RESULT_WAIT_FRAME (100)

enum STAGE // ステージパターン　
{
	NOMAL_STAGE,
	BOSS_STAGE,
	CLEAR_STAGE,
	TEST_STAGE,
};

// enum ENEMY_POP_PATTERN // 雑魚敵出現パターン　
//{
//	NOMAL_ENEMY,
//	TRACE_ENEMY,
//	CHARGE_ENEMY,
// };

class GameMainScene : public BasicScene
{
  public:
	GameMainScene();
	~GameMainScene();
	int action();
	void draw();
	void start();

  private:
	//---各クラスポインタ---
	// キャラ
	Player *mPlayer;  // プレイヤー
	BossEnemy *mBoss; // ボス敵
	// 管理関数
	BulletManager *mBulletManager;           // 弾管理クラス
	MinionEnemyManager *mMinionEnemyManager; // 雑魚敵管理クラス
	EffectManager *mEffectManager;           // エフェクト管理クラス
	ItemObjectManager *mItemObjectManager; // アイテムオブジェクト管理クラス

	//---ボス敵の演出に関わる変数---

	int mRedBGFadeOutCount;        // ボス出現演出回数カウント
	int mBossExploEFCount;         // 爆発エフェクト再生回数カウント
	int mBossExploEFIntervalCount; // 爆発エフェクト再生間隔カウント
	bool mIsBossAlert;             // ボスアラート演出+ダメージ演出判定　赤背景
	int mRedBGAlpha;               // 赤背景　透過度
	double randomExplositionPositionX; // 撃破爆発演出X座標
	double randomExplositionPositionY; // 撃破爆発演出Y座標

	//------------------------------
	//---ゲーム進行に関わる変数---
	int mScore;                 // スコア
	int mStartTime;             // 開始時間
	int mTime;                  // 経過時間
	int mMovieCount;            // 動画更新カウント
	int mResultSwichDelayCount; // リザルトシーン以降カウント
	STAGE mStageType;           // ステージパターン

	//----------------------------

	//---その他の関数---
	void playBossDefeatExplosion(); // ボス撃破時爆破演出
	int checkPlayerDefeat();        // 負け判定

	//------------------
};

#endif // !__GAMEMAINSCENE_H__