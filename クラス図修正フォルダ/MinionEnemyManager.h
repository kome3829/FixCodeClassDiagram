#ifndef __MINIONENEMYMANAGER_H__
#define __MINIONENEMYMANAGER_H__

#include "Deta.h"
#include "DxLib.h"
#include "Enemy.h"
#include "Traceenemy.h"
#include "ChargeEnemy.h"
#include "define.h"
// --- 出現座標 ---
constexpr int CHARGE_ENEMY_POP_LEFT_X = -20;
constexpr int CHARGE_ENEMY_POP_RIGHT_X = 660;
constexpr int ENEMY_POP_Y = 0;
// --- 敵出現間隔 ---
constexpr int ENEMY_POP_DIVISOR = 3;

enum ENEMY_POP_PATTERN
{
	NORMAL_ENEMY,
	TRACE_ENEMY,
	CHARGE_ENEMY,
};


class MinionEnemyManager
{
  public:
	MinionEnemyManager();
	~MinionEnemyManager();
	void action(EnemyActionData *actiondata);
	void draw();
	void start();
	Enemy *mEnemies[MAX_ENEMY_COUNT];             // 雑魚敵
	ChargeEnemy *mChargeEnemies[MAX_ENEMY_COUNT]; // チャージ攻撃敵
	TraceEnemy *mTraceEnemies[MAX_ENEMY_COUNT];   // 追従敵


	bool minionEnemyPop(); // 雑魚敵の出現パターン管理関数

  private:
	//---雑魚敵に関わる変数---

	int mEnemyNumber;   // 通常敵の数
	int mEnemyPopCount; // 通常敵の出現カウント

	int mTraceEnemyNumber;   // 追従敵数
	int mTraceEnemyPopCount; // 追従敵出現カウント

	int mChargeEnemyNumber;   // チャージ攻撃敵数
	int mChargeEnemyPopCount; // チャージ攻撃敵出現カウント

	ENEMY_POP_PATTERN mEnemyPopPattern; // 雑魚出現パターン　
	////---各雑魚敵出現関数---
	void popEnemy();
	void popTraceEnemy();
	void popChargeEnemy();
	////---各雑魚敵の撃破確認関数---
	bool checkAllEnemyDefeat();       // 通常敵が全部死んだか
	bool checkAllTraceEnemyDefeat();  // 追従敵が全部死んだか
	bool checkAllChargeEnemyDefeat(); // チャージ敵が全部死んだか
};

#endif // !__MINIONENEMYMANAGER_H__