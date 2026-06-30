#include "MinionEnemyManager.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note     各雑魚敵クラスのインスタンスを生成している
@note      各種初期設定。クラス生成の際に必要なパラメータはすべて設定する
@note      ゴミ値が入らないように各変数を初期化を行う
*/
MinionEnemyManager::MinionEnemyManager()
{
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		mEnemies[i] = new Enemy();
		mTraceEnemies[i] = new TraceEnemy();
		mChargeEnemies[i] = new ChargeEnemy();
	}
	mEnemyPopCount = 0;
	mEnemyNumber = 0;
	mTraceEnemyPopCount = 0;
	mTraceEnemyNumber = 0;
	mChargeEnemyNumber = 0;
	mChargeEnemyPopCount = 0;
	mEnemyPopPattern = NOMAL_ENEMY;
}
/*
@brief	デストラクタ

@param	なし
@return		なし

@note
インスタンス生成した 全ての雑魚敵クラスのdeleteを行い、メモリの開放を行う
*/

MinionEnemyManager::~MinionEnemyManager()
{
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		delete mEnemies[i];
		delete mTraceEnemies[i];
		delete mChargeEnemies[i];

		mEnemies[i] = nullptr;
		mTraceEnemies[i] = nullptr;
		mChargeEnemies[i] = nullptr;
	}
}
/*
@brief	メインループで実行する更新処理を行う関数

@param     	なし
@return     なし
@note		生成した全ての雑魚敵クラスの更新処理を行う
*/
void MinionEnemyManager::action(Player *player_p, int *score,
                                BulletManager *bulletManager,
                                EffectManager *effectManager,
                                ItemObjectManager *itemObjectManager
                                )
{
	// 雑魚敵の更新処理
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		mEnemies[i]->action(score, bulletManager, effectManager,
		                    itemObjectManager, player_p);
		mTraceEnemies[i]->action((int)player_p->mX, (int)player_p->mY, score,bulletManager,effectManager,itemObjectManager,player_p);
		mChargeEnemies[i]->action(score, bulletManager, player_p, effectManager,
		                          itemObjectManager);
	}
}
/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note	生成した全ての雑魚敵クラスの描画処理を行う

*/
void MinionEnemyManager::draw()
{
	// 雑魚敵の表示処理
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		mEnemies[i]->draw();
		mTraceEnemies[i]->draw();
		mChargeEnemies[i]->draw();
	}
}
/*
@brief	処理開始に必要なパラーメータの初期設定や処理を行う関数

@param	なし
@return		なし
@note	生成した全ての雑魚敵クラスの初期設定や処理を行う

*/
void MinionEnemyManager::start()
{
	// 雑魚敵の表示処理
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		mEnemies[i]->start();
		mTraceEnemies[i]->start();
		mChargeEnemies[i]->start();
	}

	mEnemyPopCount = 0;
	mEnemyNumber = 0;
	mTraceEnemyPopCount = 0;
	mTraceEnemyNumber = 0;
	mChargeEnemyNumber = 0;
	mChargeEnemyPopCount = 0;
	mEnemyPopPattern = NOMAL_ENEMY;
}



/*
@brief	通常敵の出現を管理する関数

@param		なし

@return		なし

@note      雑魚敵ステージの出現パターン(enemyPattern)がNOMALの場合に使用している
@note      定めた出現数まで出現処理を実行する
@note      enemyPopCoint変数でカウントし、一定間隔で処理を実行する
@note      出現位置は左右交互に切り替えている

*/
void MinionEnemyManager::popEnemy()
{
	mEnemyPopCount++; // 出現カウントの更新
	                  // 出現数カウントが最大数ではなく、出現カウントが90の場合
	if (mEnemyPopCount == ENEMY_POP_FRAME / ENEMY_POP_DIVISOR &&
	    mEnemyNumber != MAX_ENEMY_COUNT)
	{
		// 左右交互に出現
		// 出現数カウントが奇数or偶数で判定
		// 出現数カウントの処理は1つにまとめられそう
		if (mEnemyNumber % EVEN_NUMBER == 0)
		{
			mEnemies[mEnemyNumber]->pop(ENEMY_POP_LEFT_POSITION, 0,
			                            mEnemyNumber, false); // 敵を出現
			mEnemyNumber++; // 出現数をカウント
		}
		else
		{
			mEnemies[mEnemyNumber]->pop(ENEMY_POP_RIGHT_POSITION, 0,
			                            mEnemyNumber, false); // 敵を出現
			mEnemyNumber++; // 出現数をカウント
		}
		mEnemyPopCount = 0; // 出現カウントをリセット
	}
}
/*
@brief	追従する敵の出現を管理する関数

@param		なし

@return		なし

@note      雑魚敵ステージの出現パターン(enemyPattern)がTRACEの場合に使用している
@note      定めた出現数まで出現処理を実行する
@note      TenemyPopCoint変数でカウントし、一定間隔で処理を実行する
@note      出現位置は左右交互に切り替えている

*/
void MinionEnemyManager::popTraceEnemy()
{
	mTraceEnemyPopCount++; // 出現カウントの更新
	                       // 出現数カウントが最大数ではなく、出現カウントが90の場合
	if (mTraceEnemyPopCount == ENEMY_POP_FRAME / ENEMY_POP_DIVISOR &&
	    mTraceEnemyNumber != MAX_ENEMY_COUNT)
	{
		// 左右交互に出現
		// 出現数カウントが奇数or偶数で判定
		if (mTraceEnemyNumber % EVEN_NUMBER == 0)
		{
			mTraceEnemies[mTraceEnemyNumber]->pop(ENEMY_POP_LEFT_POSITION, 0,
			                                      mTraceEnemyNumber,
			                                      false); // 敵を出現
		}
		else
		{
			mTraceEnemies[mTraceEnemyNumber]->pop(ENEMY_POP_RIGHT_POSITION, 0,
			                                      mTraceEnemyNumber,
			                                      false); // 敵を出現
		}
		mTraceEnemyNumber++;     // 出現数をカウント
		mTraceEnemyPopCount = 0; // 出現カウントをリセット
	}
}
/*
@brief	チャージ攻撃する敵の出現を管理する関数

@param		なし

@return		なし

@note
雑魚敵ステージの出現パターン(enemyPattern)がTRACEとCHARGEの場合に使用している
@note	   定めた出現数まで出現処理を実行する
@note      CenemyPopCoint変数でカウントし、一定間隔で処理を実行する
@note      ６体連続で出現するように実行している
@note      出現位置を左右交互に切り替えている

*/
void MinionEnemyManager::popChargeEnemy()
{
	mChargeEnemyPopCount++; // 出現カウントの更新
	// 出現数カウントが最大数ではなく
	// 出現カウントが270以上かつ
	// 出現間隔がぴったりの場合(30カウント毎)
	if (mChargeEnemyPopCount >= ENEMY_POP_FRAME &&
	    mChargeEnemyPopCount % CHARGE_ENEMY_POP_INTERVAL_FRAME == 0 &&
	    mChargeEnemyNumber != MAX_ENEMY_COUNT)
	{
		// 左右交互に出現
		// 出現数カウントが奇数or偶数で判定
		// 出現数カウントの処理は1つにまとめられそう
		if (mChargeEnemyNumber % EVEN_NUMBER == 0)
		{
			mChargeEnemies[mChargeEnemyNumber]->start();
			mChargeEnemies[mChargeEnemyNumber]->pop(CHARGE_ENEMY_POP_LEFT_X, 0,
			                                        mChargeEnemyNumber,
			                                        false); // 敵を出現
			mChargeEnemyNumber++;                           // 出現数をカウント
		}
		else
		{
			mChargeEnemies[mChargeEnemyNumber]->start();
			mChargeEnemies[mChargeEnemyNumber]->pop(CHARGE_ENEMY_POP_RIGHT_X, 0,
			                                        mChargeEnemyNumber,
			                                        false); // 敵を出現
			mChargeEnemyNumber++;                           // 出現数をカウント
		}
	}
	// 出現6体分のカウントが進んだ場合（449カウント以上）
	if (mChargeEnemyPopCount >= CHARGE_ENEMY_POP_END_COUNT)
		mChargeEnemyPopCount = 0; // 出現カウントをリセット
}
/*
@brief	全ての通常敵の撃破判定の確認を行う関数

@param		なし

@return		撃破判定:bool:全て倒された true/まだ倒されていない敵がいる　false

@note     　1匹でも残っていたら失敗falseを返している
@note       出現パターンの切り替えで使用している

*/
bool MinionEnemyManager::checkAllEnemyDefeat()
{
	if (mEnemyNumber < MAX_ENEMY_COUNT)
	{
		return false;
	}
	// 最大出現数繰り返す
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		if (mEnemies[i]->mIsDefeat)
		{
			continue;
		}
		else
		{
			return false; // 負けていない敵がいればfalse
		}
	}

	return true;
}
/*
@brief	全ての追従する敵の撃破判定の確認を行う関数

@param		なし

@return		撃破判定:bool:全て倒された true/まだ倒されていない敵がいる　false

@note      1匹でも残っていたら失敗falseを返している
@note      出現パターンの切り替えで使用している

*/
bool MinionEnemyManager::checkAllTraceEnemyDefeat()
{
	if (mTraceEnemyNumber < MAX_ENEMY_COUNT)
	{
		return false;
	}
	// 最大出現数繰り返す
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		if (mTraceEnemies[i]->mIsDefeat)
		{
			continue;
		}
		else
		{
			return false; // 負けていない敵がいればfalse
		}
	}

	return true;
}
/*
@brief	全てのチャージ攻撃する敵の撃破判定の確認を行う関数

@param		なし

@return		撃破判定:bool:全て倒された true/まだ倒されていない敵がいる　false

@note      1匹でも残っていたら失敗falseを返している
@note      出現パターンの切り替えで使用している

*/
bool MinionEnemyManager::checkAllChargeEnemyDefeat()
{
	if (mChargeEnemyNumber < MAX_ENEMY_COUNT)
	{
		return false;
	}
	// 最大出現数繰り返す
	for (int i = 0; i < MAX_ENEMY_COUNT; i++)
	{
		if (mChargeEnemies[i]->mIsDefeat)
		{
			continue;
		}
		else
		{
			return false; // 負けていない敵がいればfalse
		}
	}

	return true;
}

/*
@brief	雑魚敵の出現パターンを管理を行う

@param	なし
@return		なし
@note	撃破判定関数の実行によって出現パターンを切り替えている
@note   出現パターン(mEnemyPattern)に応じて処理を分岐している
        1. CHARGE　チャージ攻撃敵を出現
        2. NOMAL　 通常敵を出現
        3. TRACE	 追従敵とチャージ敵を出現

*/
bool MinionEnemyManager::minionEnemyPop()
{
	switch (mEnemyPopPattern) // 出現パターンで切り替え
	{
	case CHARGE_ENEMY: // チャージ敵出現パターン
		// チャージ敵の出現処理
		popChargeEnemy();
		// 全ての敵の撃破確認。撃破が確認できると次の出現パターン
		if (checkAllChargeEnemyDefeat())
		{
			// 出現パターンを変更、チャージ敵出現数カウントの初期化
			mEnemyPopPattern = TRACE_ENEMY;
			mChargeEnemyNumber = 0;
		}

		break;

	case NOMAL_ENEMY: // 通常敵出現パターン
		// 通常敵の出現処理
		popEnemy();
		// 全ての敵の撃破確認。撃破が確認できると次の出現パターン
		if (checkAllEnemyDefeat())
		{
			mEnemyPopPattern = CHARGE_ENEMY; // 出現パターンを変更
		}

		break;

	case TRACE_ENEMY: // 追従敵出現パターン
		// 追従敵、チャージ敵の出現処理
		popChargeEnemy();
		popTraceEnemy();

		// 全ての敵の撃破確認。撃破が確認できるとボス出現演出
		if (checkAllTraceEnemyDefeat() && checkAllChargeEnemyDefeat())
		{
			return true;
		}

		break;

	default:
		break;
	}
	return false;
}
