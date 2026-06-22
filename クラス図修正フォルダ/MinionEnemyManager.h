#pragma once
#include "DxLib.h"
#include "define.h"
#include "deta.h"
#include "enemy.h"
#include "chargeEnemy.h"
#include "Traceenemy.h"
#include "EffectManager.h"
#include "ItemObjectManager.h"

enum ENEMY_POP_PATTERN 
{
  NOMAL_ENEMY,
  TRACE_ENEMY,
  CHARGE_ENEMY,
};


class MinionEnemyManager {
public:
  MinionEnemyManager();
  ~MinionEnemyManager();
  void action();
  void draw();
  void start();

  bool takeDamageMinionEnemy(EffectManager* effectManager_p, int* score,
                  ItemObjectManager* itemobjectManager_p); // ダメージ処理
  void shotMinionEnemyBullet(BulletManager* bulletManager_p,EffectManager* effectManager_p,
                        ItemObjectManager* itemObjectManager_p); // 弾発射関数



  Enemy *mEnemies[MAX_ENEMY_COUNT];             // 雑魚敵
  ChargeEnemy *mChargeEnemies[MAX_ENEMY_COUNT]; // チャージ攻撃敵
  TraceEnemy *mTraceEnemies[MAX_ENEMY_COUNT];   // 追従敵
  
  //---雑魚敵に関わる変数---

  int mEnemyNumber;   // 通常敵の数
  int mEnemyPopCount; // 通常敵の出現カウント

  int mTraceEnemyNumber;   // 追従敵数
  int mTraceEnemyPopCount; // 追従敵出現カウント

  int mChargeEnemyNumber;   // チャージ攻撃敵数
  int mChargeEnemyPopCount; // チャージ攻撃敵出現カウント

  ENEMY_POP_PATTERN mEnemyPopPattern; //雑魚出現パターン　

  //---各雑魚敵出現関数---
  void popEnemy();
  void popTraceEnemy();
  void popChargeEnemy();
  //---
  // 
  //---各雑魚敵の撃破確認関数---
  bool checkAllEnemyDefeat();       //通常敵が全部死んだか
  bool checkAllTraceEnemyDefeat();  //追従敵が全部死んだか
  bool checkAllChargeEnemyDefeat(); //チャージ敵が全部死んだか
  //---

  void MinionEnemyPop(); // 雑魚敵の出現パターン管理関数

private:
};
