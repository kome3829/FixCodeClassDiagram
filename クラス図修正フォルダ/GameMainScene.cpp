#include "GameMainScene.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note      各種初期設定。クラス生成の際に必要なパラメータはすべて設定する
@note      ゴミ値が入らないように各変数を初期化を行う
@note      各オブジェクトのインスタンスを生成している

*/
GameMainScene::GameMainScene()
{
	mPlayer = new Player();
	mBoss = new BossEnemy();
	mBulletManager = new BulletManager();
	mMinionEnemyManager = new MinionEnemyManager();
	mEffectManager = new EffectManager();
	mItemObjectManager = new ItemObjectManager();
	mStageType = NOMAL_STAGE;
	mBossExploEFCount = 0;
	mBossExploEFIntervalCount = 0;
	mScore = 0;
	mTime = 0;
	mStartTime = GetNowCount();
	mResultSwichDelayCount = 0;
	mMovieCount = 0;
	mIsBossAlert = false;
	mRedBGFadeOutCount = 0;
	mRedBGAlpha = MAX_ALPHA;
	randomExplositionPositionX = 0;
	randomExplositionPositionY = 0;
}

/*
@brief	デストラクタ

@param	なし
@return		なし

@note      メモリ開放処理を忘れており、空の内容で作成されている
@note
インスタンス生成した各オブジェクトのdeleteを行い、メモリの開放を行う必要がある
*/
GameMainScene::~GameMainScene()
{
}

/*
@brief
メインループ内で動作させる1フレームの更新処理をまとめた関数。返り値によってシーン遷移を行う

@param[in]	なし

@return		シーンナンバー：int

@note		タイトルシーン時/    	SCENE_TITLE　  0
@note		ゲームメインシーン時/	SCENE_GAMEMAIN 1
@note		リザルトシーン時/   	SCENE_RESULT   2

@note

  - 処理内容

      - 背景動画のループ再生
          - 一定フレームになると背景動画の再生位置を先頭へ戻してループ再生

      - テスト用ステージ処理
        -
ボス出現演出やボスの行動パターンの確認をするために対応するパラメータを直接設定している

      - ゲーム内時間の更新
        - 現在の時間と開始時の時間の差から経過時間を計算している

      - 各オブジェクトの更新処理
        - プレイヤー・敵・ボス・弾・アイテムのaction関数を実行する
        - 敵・弾・アイテムはfor文で複数実行している

      - 弾の発射処理
        - プレイヤー・敵・ボスの弾を発射する関数を実行する
        - 敵はfor文で複数実行している

      - 当たり判定処理
        - プレイヤー・敵・ボスの当たり判定関数を実行する
        - プレイヤー・ボスの当たり判定関数はfor文で弾の数に応じて複数実行する
        - 敵の当たり判定関数は2重のfor文で敵の数と弾の数に応じて複数実行する
        - 敵の当たり判定関数ではエフェクトの表示、アイテムドロップも行っている

      - アイテム取得処理
        - プレイヤーのアイテム取得関数を実行する
        -
アイテム取得関数の返り値応じて、アイテム取得エフェクトの表示を分岐している
        - for分で複数実行している

      - ステージ進行の処理
        - ステージに応じた処理を現在のステージ(stage)に応じて分岐している
        1. TEST　
           - テスト用のステージ。処理は別でしているので記載なし
        2. NOMAL
           - 雑魚敵のステージ。 それぞれの敵の出現関数を実行する
           - 出現パターン(enemyPattern)に応じて更に処理を分岐している
           - 撃破判定関数の実行によって出現パターンを切り替えている
              1. CHARGE　チャージ攻撃敵を出現
              2. NOMAL　 通常敵を出現
              3. TRACE	 追従敵とチャージ敵を出現
        3. BOSSSTAGE　
            - ボスステージ。
            - ボス撃破演出関数の実行。関数内の処理でクリアステージへ切り替える
        4. CLEAR
            -
クリアステージ。action関数の返り値をSCENE_RESULTにして、シーン遷移を行う

      - プレイヤーの撃破判定処理
        - プレイヤーの撃破判定関数を実行する
        -
プレイヤーの撃破判定関数の返り値がそのままaction関数の返り値として、シーン遷移を行う

  @note　2重ループのfor分には変数ｊではなくkを使用している。iとjが見分けずらいので。３重の場合はlを使用
*/
int GameMainScene::action()
{
	// テストステージ用の処理
	if (mStageType == TEST_STAGE)
	{
		// ボス敵出現演出の確認
		mRedBGAlpha -= FADE_OUT_SPEED;
		if (mRedBGAlpha < 0)
		{
			mRedBGFadeOutCount++;
			mRedBGAlpha = MAX_ALPHA;
		}
		if (mRedBGFadeOutCount >= REDBG_FADEOUT_COUNT_MAX)
		{
			mIsBossAlert = false;
			mStageType = BOSS_STAGE;
			mBoss->mIsActive = true; // ボス出現
			// mBoss->mHitPoint = TEST_BOSS_HP;
			// boss->HP = BOSS_MAX_HP;

			mPlayer->mShotPower = TEST_PLAYER_POWER;
			mRedBGAlpha = MAX_ALPHA;
		}
		// 無敵の確認
		mPlayer->mIsUnbeatable = true;
		// エフェクト表示の確認
		if (checkSinglePushKey(KEY_INPUT_SPACE))
		{
			mEffectManager->setEffect(&(mPlayer->mX), &(mPlayer->mY),
				                           WARP_EF);
		}
	}
	// 動画ループ再生
	mMovieCount++;                       // 動画用カウントの更新
	if (mMovieCount >= MOVIE_LOOP_FRAME) // 600フレーム(１０秒)でループ再生
	{
		SeekMovieToGraph(Data::getInstance()->mBackGroundMovieHandle,
		                 MOVIE_LOOP_START);
		mMovieCount = 0; // 動画用カウントのリセット
	}

	PlayMovieToGraph(Data::getInstance()->mBackGroundMovieHandle);

	// ゲーム内時間の更新
	mTime = (GetNowCount() - mStartTime);

	// プレイヤーの更新処理、発射処理
	mPlayer->action(mBulletManager,mEffectManager,&mScore);

	// ボスの更新処理、発射処理
	mBoss->action(&mScore,mBulletManager,mEffectManager,mItemObjectManager,mMinionEnemyManager,mPlayer);

	// 弾管理クラスの当たり判定関数管理
	mBulletManager->checHit(mPlayer, mBoss, mMinionEnemyManager);

	//アイテムクラスの当たり判定管理
	mItemObjectManager->hitCheck(mPlayer);
	
	// 管理クラスの更新処理
	mBulletManager->action();
	mMinionEnemyManager->action(mPlayer, &mScore, mBulletManager,mEffectManager,mItemObjectManager);
	mItemObjectManager->action();


	// ステージごとの更新処理
	switch (mStageType)
	{
	case TEST_STAGE:
		// PopCenemy();
		break;

	case NOMAL_STAGE: // 雑魚敵ステージ
		if (mMinionEnemyManager->MinionEnemyPop())
		{
			mIsBossAlert = true;
		}

		if (mIsBossAlert)
		{
			// アルファ値を減らしフェイドアウト
			mRedBGAlpha -= FADE_OUT_SPEED;

			// フェイドアウトを繰り返す
			if (mRedBGAlpha < 0)
			{
				mRedBGFadeOutCount++;
				mRedBGAlpha = MAX_ALPHA;
			}

			// 演出終わり、出現の初期設定を行う
			if (mRedBGFadeOutCount >= REDBG_FADEOUT_COUNT_MAX)
			{
				mIsBossAlert = false;
				mStageType = BOSS_STAGE; // ボスステージに変更
				mBoss->mIsActive = true; // ボス出現
				mRedBGAlpha = MAX_ALPHA;
			}
		}

		break;

	case BOSS_STAGE: // ボスステージ

		// ボス撃破の判定、撃破演出処理
		if (mBoss->mIsDefeat)
		{
			playBossDefeatExplosion(); // 撃破演出関数内の処理でステージを切り替え
		}
		break;

	case CLEAR_STAGE:
		// リザルトシーン切り替え
		return SCENE_RESULT;
		break;

	default:
		break;
	}
	// プレイヤー撃破判定関数の返り値でシーン切り替え
	return checkPlayerDefeat();
}

/*
@brief	描画ループで実行される描画処理をまとめた関数

@param	なし

@return		なし

@note

      - 背景動画の表示
      - 各オブジェクトのdraw関数及びanimePlay関数を実行
      - スコア、時間、ショットパワーなどのUIを表示
      - ボスステージの場合のみボスのHPバーを表示
      - プレイヤーが撃破された場合のみ、ゲームオーバーのテロップ画像を表示
      - ダメージ表現を画面サイズの真っ赤な画像を表示で行う
        - アルファブレンドで素早くフェイドアウト
        - ボス出現の演出でも使用
@warning
上から順に表示していくので、奥から手前に表示する順番を考えて処理を実行している
プレイヤーのダメージ表現は重要なので画面全体でわかりやすく行う
*/
void GameMainScene::draw()
{
	// 背景動画の表示
	DrawGraph(SCREEN_ORIGIN_POINT, SCREEN_ORIGIN_POINT,
	          Data::getInstance()->mBackGroundMovieHandle, true);

	// ボスの表示
	mBoss->draw();
	// エフェクトの表示
	mEffectManager->playEffectAnimation();
	mMinionEnemyManager->draw();

	// 管理クラスの表示処理
	mBulletManager->draw();

		mItemObjectManager->draw();


	// プレイヤーの表示
	mPlayer->draw();

	// UI表示
	SetFontSize(UI_FONT_SIZE);
	// テキストボックス
	DrawBox(SCREEN_ORIGIN_POINT, UI_BOX_TOP, MAX_SCREEN_WIDTH, UI_BOX_BOTTOM,
	        COLOR_BLACK, TRUE);
	// スコアのテキスト表示
	DrawFormatString(UI_SCORE_X, UI_BOX_TOP, COLOR_WHITE, "SCORE:%d", mScore);
	// ショットパワーの表示
	DrawFormatString(UI_SHOT_POWER_X, UI_BOX_TOP, COLOR_WHITE, "shootPower:%d",
	                 mPlayer->mShotPower);
	// 秒に変換
	double timeSecond = (double)mTime / TIME_MILLISECOND_TO_SECOND;
	// タイムのテキスト表示
	DrawFormatString(UI_TIME_X, UI_BOX_TOP, COLOR_WHITE, "TIME:%f", timeSecond);

	// ボスステージのみボスHPバーの表示
	if (mStageType == BOSS_STAGE)
	{
		SetFontSize(UI_FONT_SIZE);
		// HPパーセントのテキスト表示
		DrawFormatString(BOSS_HP_TEXT_X, BOSS_HP_TEXT_Y, COLOR_WHITE, "%d",
		                 mBoss->mBossHitPointPercent);
		// 外枠
		DrawBox(BOSS_HP_BAR_LEFT_X, BOSS_HP_BAR_TOP_Y, BOSS_HP_BAR_RIGHT_X,
		        BOSS_HP_BAR_BOTTOM_Y, COLOR_GREEN, TRUE);
		// HPバー、残りHPによって右端座標が変わる
		DrawBox(BOSS_HP_INNER_LEFT_X, BOSS_HP_INNER_TOP_Y,
		        mBoss->mBossHitPointBarRightEnd, BOSS_HP_INNER_BOTTOM_Y,
		        COLOR_RED, TRUE);

		SetFontSize(UI_FONT_SIZE);
	}
	// プレイヤー撃破時
	if (mPlayer->mIsDefeat)
	{
		// ゲームオーバーテロップを表示
		DrawGraph(SCREEN_ORIGIN_POINT, SCREEN_ORIGIN_POINT,
		          Data::getInstance()->mGameOverTelopImageHandle, TRUE);
	}

	// ダメージ表現 、ボスの出現演出
	if (mIsBossAlert)
	{
		// 画面全体を真っ赤にする。素早くフェイドアウト
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, mRedBGAlpha); // 透過度設定
		DrawGraph(SCREEN_ORIGIN_POINT, SCREEN_ORIGIN_POINT,
		          Data::getInstance()->mRedBackGroundImageHandle,
		          TRUE); // アルファ値を反映した画面の表示
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 透過度のリセット
	}
}

/*
@brief	動作開始時に必要なパラーメータの初期設定や処理を行う関数

@param		なし

@return		なし

@note      再生しているBGMを止めてゲーム本編BGMを再生している
@note      各パラメータの初期設定を行っている
@note      各オブジェクトのstart関数を実行している

*/
void GameMainScene::start()
{
	// BGMの停止、ゲーム本編BGM再生
	StopSoundMem(Data::getInstance()->mTitleBgmHandle);
	StopSoundMem(Data::getInstance()->mGameBgmHandle);
	PlaySoundMem(Data::getInstance()->mGameBgmHandle, DX_PLAYTYPE_LOOP);

	mBossExploEFCount = 0;
	mBossExploEFIntervalCount = 0;
	mResultSwichDelayCount = 0;
	mScore = 0;
	mTime = 0;
	mStartTime = GetNowCount();
	mRedBGAlpha = MAX_ALPHA;
	mRedBGFadeOutCount = 0;
	// NOMAL_STAGE,TEST_STAGE;
	mStageType = NOMAL_STAGE;
	mIsBossAlert = false;
	randomExplositionPositionX = 0;
	randomExplositionPositionY = 0;
	
	// 各インスタンスの初期化処理
	mPlayer->start();
	mBoss->start();
	// 管理クラスの初期化処理
	mBulletManager->start();
	mMinionEnemyManager->start();
	mEffectManager->start();
	mItemObjectManager->start();

}





/*
@brief	ボス撃破演出を行う関数

@param[in]	なし

@return		なし

@note      爆発SEと爆破エフェクトの再生を一定回数繰り返す
@note      爆破エフェクトはボス上のランダムな位置で再生
@note
一定回数の演出が終わるとゲームスコア、時間をdataクラス内変数に保存し、クリアステージへ切り替える

*/
void GameMainScene::playBossDefeatExplosion()
{
	// 最大爆破数まで繰り返す
	if (mBossExploEFCount < BOSS_MAX_BOMB_COUNT)
	{
		mBossExploEFIntervalCount++; // 間隔カウントの更新

		// 15フレーム間隔で実行
		if (mBossExploEFIntervalCount >= DEFEAT_EXPLOSION_INTERVAL_FRAME)
		{
			// 撃破SEの再生
			mBoss->playSoundEffect();
			// 間隔カウントリセット
			mBossExploEFIntervalCount = 0;

							// ボス画像上のランダムな位置を計算
			randomExplositionPositionX =
			    (int)mBoss->mX + BOSS_DEFEAT_EXPLOSION_OFFSET_X +
			    GetRand(BOSS_DEFEAT_EXPLOSION_RANDOM_RANGE) *
			        BOSS_DEFEAT_EXPLOSION_STEP;
			randomExplositionPositionY =
			    (int)mBoss->mY + BOSS_DEFEAT_EXPLOSION_OFFSET_Y +
			    GetRand(BOSS_DEFEAT_EXPLOSION_RANDOM_RANGE) *
			        BOSS_DEFEAT_EXPLOSION_STEP;

			mEffectManager->setEffect( &(randomExplositionPositionX),
			                           &(randomExplositionPositionY),
			                          EXPLOSION_EF);
			
			mBossExploEFCount++; // 爆破エフェクト数カウントの更新
		}
	}

	if (mBossExploEFCount == BOSS_MAX_BOMB_COUNT) // 最大数まで繰り返したら
	{
		// ボスのリセット
		mBoss->start();
		// スコアの保存
		Data::getInstance()->mResultGameScore = mScore;

		int timeSeconds;
		int timeMinutes;
		// 分と秒に変換
		timeSeconds =
		    (mTime / TIME_MILLISECOND_TO_SECOND) % TIME_SECOND_PER_MINUTE;

		timeMinutes =
		    (mTime / TIME_MILLISECOND_TO_SECOND) / TIME_SECOND_PER_MINUTE;

		// 時間の保存
		Data::getInstance()->mInGameTimeSecond = timeSeconds;
		Data::getInstance()->mInGameTimeMinutes = timeMinutes;

		mStageType = CLEAR_STAGE;
	}
}

/*
@brief	プレイヤーの撃破判定を行う関数

@param[in]	なし

@return		シーンナンバー：int

@note		タイトルシーン時/    SCENE_TITLE　  0
@note		ゲームメインシーン時/SCENE_GAMEMAIN 1
@note		リザルトシーン時/    SCENE_RESULT   2

@note

- プレイヤー撃破フラグ(dethFlg)がtrueの場合のみ処理を実行する
- ゲームスコア、時間をdataクラス内変数に保存を行う
- 返り値SCENE_RESULTを返し、シーン遷移を行う
-
プレイヤー撃破フラグ(dethFlg)がfalseの場合はSCENE_GAMEMAINを返り値にしてゲームシーンを続ける
@warning
- 判定を行った戻り値でそのままシーン遷移を行えるようにした
*/
int GameMainScene::checkPlayerDefeat()
{
	// 死亡判定
	if (mPlayer->mIsDefeat)
	{
		mResultSwichDelayCount++; // リセットカウントの更新

		// 100フレームで実行
		if (mResultSwichDelayCount == PLAYER_DEFEAT_RESULT_WAIT_FRAME)
		{
			// BGMを止める
			StopSoundMem(Data::getInstance()->mGameBgmHandle);
			// スコアの保存
			Data::getInstance()->mResultGameScore = mScore;
			// 分と秒に変換
			int timeSeconds;
			int timeMinutes;
			timeSeconds =
			    (mTime / TIME_MILLISECOND_TO_SECOND) % TIME_SECOND_PER_MINUTE;
			timeMinutes =
			    (mTime / TIME_MILLISECOND_TO_SECOND) / TIME_SECOND_PER_MINUTE;
			// 時間の保存
			Data::getInstance()->mInGameTimeSecond = timeSeconds;
			Data::getInstance()->mInGameTimeMinutes = timeMinutes;
			// リザルトシーンへ切り替え
			return SCENE_RESULT;
		}
	}
	return SCENE_GAMEMAIN;
}
