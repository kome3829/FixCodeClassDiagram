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
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i] = new Object();
		mExplosions[i] = new Explosion();
		mEffects[i] = new Effect();
	}
	mStageType = NOMAL_STAGE;
	mNomalShotIntervalCount = 0;
	mNomalShotInterval = PLAYER_NORMAL_SHOT_SPEED;
	mMissileShotIntervalCount = 0;
	mSpecialShotCount = 0;
	mPlayerShotCount = 0;
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
			for (int i = 0; i < MAX_BULLET_NUMBER; i++)
			{
				if (mEffects[i]->setEffect(&(mPlayer->mX), &(mPlayer->mY),
				                           WARP_EF))
				{
					break;
				}
			}
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
	mPlayer->action();
	shotPlayerBullet((int)mPlayer->mX, (int)mPlayer->mY, mPlayer->mShotPower);
	// ボスの更新処理、発射処理
	mBoss->action(&mScore);
	shotBossEnemyBullet(mBoss);

	// 弾管理クラスの当たり判定関数管理
	mBulletManager->checHit(mPlayer, mBoss);
	//mBulletManager->checHit(mPlayer, mBoss, mMinionEnemyManager);


	// ボス、プレイヤーの当たり判定処理
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		// 弾との当たり判定
		//  プレイヤー弾とボス敵の当たり判定

		// アイテムの当たり判定(プレイヤー)
		switch (mPlayer->checkItemObjectHit(
		    mItemObjects[i], &mScore)) // 当たったオブジェクトを返す
		{
		case OBJECT_LIFE: // 回復アイテム
			// フラグのたっていないものを探し、１つだけ実行
			for (int k = 0; k < MAX_BULLET_NUMBER; k++)
			{
				// 獲得時のエフェクトを表示
				if (mEffects[k]->setEffect(&(mPlayer->mX), &(mPlayer->mY),
				                           LIFE_EF))
				{
					break;
				}
			}
			break;

		case OBJECT_STAR: // 無敵アイテム
			// フラグのたっていないものを探し、１つだけ実行
			for (int k = 0; k < MAX_BULLET_NUMBER; k++)
			{
				// 獲得時のエフェクトを表示
				if (mEffects[k]->setEffect(&(mPlayer->mX), &(mPlayer->mY),
				                           STAR_EF))
				{
					break;
				}
			}
			break;

		default:
			break;
		}
	}

	// 管理クラスの更新処理
	mBulletManager->action();
	mMinionEnemyManager->action(mPlayer, &mScore);

	// 各弾の更新処理
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i]->action();
	}
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
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mEffects[i]->playEffectAnimation();
	}
	mMinionEnemyManager->draw();

	// 管理クラスの表示処理
	mBulletManager->draw();

	// 各弾、アイテム、爆破エフェクトの表示
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mExplosions[i]->playExplosionAnimation();
		mItemObjects[i]->draw();
	}
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

	mNomalShotIntervalCount = 0;
	mNomalShotInterval = PLAYER_NORMAL_SHOT_SPEED;
	mMissileShotIntervalCount = 0;
	mSpecialShotCount = 0;
	mPlayerShotCount = 0;
	mBossExploEFCount = 0;
	mBossExploEFIntervalCount = 0;
	mResultSwichDelayCount = 0;
	mScore = 0;
	mTime = 0;
	mStartTime = GetNowCount();
	mRedBGAlpha = MAX_ALPHA;
	mRedBGFadeOutCount = 0;
	// NOMALSTAGE,TEST;
	mStageType = NOMAL_STAGE;
	mIsBossAlert = false;

	// 各インスタンスの初期化処理
	mPlayer->start();
	mBoss->start();
	// 管理クラスの初期化処理
	mBulletManager->start();
	mMinionEnemyManager->start();

	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i]->start();
		mExplosions[i]->start();
		mEffects[i]->start();
	}
}

/*
@brief	通常敵の弾発射を管理する関数

@param[in]	enemy* en:通常敵クラスのインスタンスポインタ

@return		なし

@note      弾を発射する敵のフラグ(FLG)がtrueの場合のみ処理を実行する
@note
enemyクラスメンバのshotCount変数でカウントし、一定間隔で弾を発射を繰り返している
@note      角度を変えながら、薙ぎ払うように連続で発射している
@note      弾発射と同時に発射エフェクト表示と発射SEの再生も行う
*/


/*
@brief	追従する敵の弾発射を管理する関数

@param[in]	Traceenemy* Ten:追従する敵クラスのインスタンスポインタ

@return		なし

@note      弾を発射する敵のフラグ(FLG)がtrueの場合のみ処理を実行する
@note
traceEnemyクラスメンバのshotCount変数でカウントし、一定間隔で弾の発射を繰り返している
@note      ４方向から同時に弾を発射する
@note      角度を変えながら、薙ぎ払うように連続で発射している
@note      弾発射と同時に発射エフェクト表示と発射SEの再生も行う

@note　2重ループのfor分には変数ｊではなくkを使用している。iとjが見分けずらいので。３重の場合はlを使用

*/
void GameMainScene::shotTraceEnemyBullet(TraceEnemy *traceEnemy)
{
	if (traceEnemy->mIsActive)
	{
		traceEnemy->mShotCount++; // 発射カウントの更新

		// 60フレームから100フレームの間で10フレーム間隔で処理を実行(計5回)
		if (traceEnemy->mShotCount >= TRACE_ENEMY_SHOT_START_FRAME &&
		    traceEnemy->mShotCount % TRACE_ENEMY_SHOT_INTERVAL_FRAME == 0 &&
		    traceEnemy->mShotCount <= TRACE_ENEMY_SHOT_END_FRAME)
		{
			// 4方向に同時に発射
			for (int i = 0; i < TRACE_ENEMY_SHOT_WAY_COUNT; i++)
			{
				// ループ回数から発射角度９０度ずつを変更
				int setAngleDistance = (i + 1) * TRACE_ENEMY_SHOT_BASE_ANGLE;

				/*mBulletManager->setBullet(
				    (int)traceEnemy->mX, (int)traceEnemy->mY,
				    setAngleDistance + traceEnemy->mShotCount -
				        TRACE_ENEMY_SHOT_START_FRAME,
				    ENEMY_NOMAL, true);*/

				// フラグのたっていないものを探し、1つだけsetef関数実行
				for (int k = 0; k < MAX_BULLET_NUMBER; k++)
				{
					// 発射エフェクトの表示
					if (mEffects[k]->setEffect(&(traceEnemy->mX),
					                           &(traceEnemy->mY), SHOT_EF))
					{
						break;
					}
				}
			}
			// 発射SEの再生
			PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
			             DX_PLAYTYPE_BACK, TRUE);
		}
		// カウントのリセット、180フレームで1巡
		if (traceEnemy->mShotCount >= TRACE_ENEMY_SHOT_RESET_FRAME)
		{
			traceEnemy->mShotCount = 0;
		}
	}
}

/*
@brief	チャージ攻撃敵の弾発射を管理する関数

@param[in]	chargeEnemy* Cen:チャージ攻撃する敵クラスのインスタンスポインタ

@return		なし

@note

- 弾を発射する敵のフラグ(FLG)がtrueの場合のみ処理を実行する
- chargeEnemyクラスメンバのshotCount変数でカウントし、処理を管理している
- 発射パターンをステージ応じて切り替えている

1. 通常のステージ
    - 一定間隔でミニ弾の発射を繰り返している
    - プレイヤーを狙うようにミニ弾を連続で発射している
    - 発射するまでの間チャージエフェクト表示を行う

2. ボスステージ
    - ミニ弾の扇状の弾幕をプレイヤーを狙うように発射している
        - 一定間隔まで発射しないチャージ時間を設けている
        - 同時に３方向ミニ弾を発射している
        - 発射を開始すると１８回連続でミニ弾の弾幕を発射する
    - 通常弾の円状の弾幕を発射している
        - 一定間隔まで発射しないチャージ時間を設けている
        - 同時に10方向に通常弾を発射している
        - チャージが終わると間隔を空けて３回の発射を行う
    - 発射するまで間のチャージエフェクトとワープエフェクトの表示を行う
    -無敵アイテムを発射開始時にドロップしている

- 弾発射と同時に発射エフェクトの表示と発射SEの再生も行う
@warning
    - ボスステージではボスのチャージ攻撃と合わせて激しい攻撃を行う
    - 救済として無敵アイテムをドロップしている

@note　2重ループのfor分には変数ｊではなくkを使用している。iとjが見分けずらいので。３重の場合はlを使用

*/
void GameMainScene::shotChargeEnemyBullet(ChargeEnemy *chargeEnemy)
{
	if (chargeEnemy->mIsActive)
	{
		// 通常ステージの処理
		if (!chargeEnemy->mIsBossStage)
		{
			chargeEnemy->mShotCount++; // 発射カウントの更新
			// 40～120フレームは溜め演出
			// 40フレームごとに処理を実行
			if (chargeEnemy->mShotCount % CHARGE_EF_INTERVAL == 0 &&
			    chargeEnemy->mShotCount <= CHARGE_EF_END)
			{
				// フラグのたっていないものを探し、1つだけsetef関数実行
				for (int i = 0; i < MAX_BULLET_NUMBER; i++)
				{
					// チャージエフェクトを表示
					if (mEffects[i]->setEffect(&(chargeEnemy->mX),
					                           &(chargeEnemy->mY), CHARGE_EF))
					{
						break;
					}
				}
			}

			// 120フレーム後なら発射処理
			if (chargeEnemy->mShotCount >= CHARGE_ENEMY_SHOT_START_FRAME)
			{
				// 10フレームの間隔を空けて発射処理
				if (chargeEnemy->mShotCount %
				        CHARGE_ENEMY_SHOT_INTERVAL_FRAME ==
				    0)
				{
					// プレイヤー狙いの角度を計算
					double vectorTargetX = (mPlayer->mX - chargeEnemy->mX);
					double vectorTargetY = (mPlayer->mY - chargeEnemy->mY);
					double targetAngle = atan2(vectorTargetY, vectorTargetX);
					double targetAngle_degPre = targetAngle * 180.0f / PI;

					//mBulletManager->setBullet(
					//    (int)chargeEnemy->mX, (int)chargeEnemy->mY,
					//    targetAngle_degPre, ENEMY_MINI_ORANGE, false);

					// フラグのたっていないものを探し、1つだけsetef関数実行
					for (int i = 0; i < MAX_BULLET_NUMBER; i++)
					{
						// 発射エフェクトの表示
						if (mEffects[i]->setEffect(&(chargeEnemy->mX),
						                           &(chargeEnemy->mY), SHOT_EF))
						{
							break;
						}
					}
					// 発射SEの再生
					PlaySoundMem(
					    Data::getInstance()->mEnemyShotSoundEffectHandle,
					    DX_PLAYTYPE_BACK, TRUE);
				}
				// 6発分のフレームカウントが進行した場合(170以上)
				if (chargeEnemy->mShotCount >= CHARGE_ENEMY_SHOT_RESET_FRAME)
				{
					chargeEnemy->mShotCount = 0; // カウントをリセット
				}
			}
		}

		// ボスステージの処理
		else
		{
			// 移動カウントが240フレームまでチャージエフェクトの表示
			if (chargeEnemy->mMoveCount <= CHARGE_ENEMY_BOSS_CHARGE_END)
			{
				// エフェクト再生終了間隔と同じ感覚でエフェクトの表示を行う(40フレームごと)
				if (chargeEnemy->mMoveCount % CHARGE_EF_INTERVAL == 0 &&
				    chargeEnemy->mMoveCount >= CHARGE_EF_INTERVAL)
				{
					// フラグのたっていないものを探し、1つだけsetef関数実行
					for (int i = 0; i < MAX_BULLET_NUMBER; i++)
					{
						// エフェクトの表示
						if (mEffects[i]->setEffect(&(chargeEnemy->mX),
						                           &(chargeEnemy->mY),
						                           CHARGE_EF))
						{
							break;
						}
					}
				}
			}

			// 移動カウントが304フレームでワープのエフェクト表示
			if (chargeEnemy->mMoveCount == CHARGE_ENEMY_BOSS_WARP_EF_FRAME)
			{
				// フラグのたっていないものを探し、1つだけsetef関数実行
				for (int i = 0; i < MAX_BULLET_NUMBER; i++)
				{
					// ワープエフェクトの表示
					if (mEffects[i]->setEffect(&(chargeEnemy->mX),
					                           &(chargeEnemy->mY), WARP_EF))
					{
						break;
					}
				}
			}
			// ワープエフェクトの表示終了フレーム(330)から攻撃処理開始
			if (chargeEnemy->mMoveCount > CHARGE_ENEMY_BOSS_ATTACK_START)
			{
				chargeEnemy->mShotCount++; // 発射カウント更新
				// 攻撃開始から１４フレーム後救済措置無敵アイテムをドロップ左右に一個ずつ
				if (chargeEnemy->mMoveCount ==
				    CHARGE_ENEMY_BOSS_ITEM_DROP_FRAME)
				{
					if (chargeEnemy->mNumber == SUMMON_LEFT)
					{
						// フラグのたっていないものを探し、1つだけset関数実行
						for (int i = 0; i < MAX_BULLET_NUMBER; i++)
						{
							// 無敵アイテムを出現
							if (mItemObjects[i]->setItemObject(
							        ITEM_DROP_LEFT_X, ITEM_DROP_Y, 0,
							        OBJECT_STAR, &(mPlayer->mX),
							        &(mPlayer->mY)))
							{
								break;
							}
						}
					}
					else if (chargeEnemy->mNumber == SUMMON_RIGHT)
					{
						// フラグのたっていないものを探し、1つだけset関数実行
						for (int i = 0; i < MAX_BULLET_NUMBER; i++)
						{
							// 無敵アイテムを出現
							if (mItemObjects[i]->setItemObject(
							        ITEM_DROP_RIGHT_X, ITEM_DROP_Y, 0,
							        OBJECT_STAR, &(mPlayer->mX),
							        &(mPlayer->mY)))
							{
								break;
							}
						}
					}
				}
				// 50フレーム間隔で実行
				if (chargeEnemy->mShotCount % CHARGE_ENEMY_RING_SHOT_INTERVAL ==
				    0)
				{
					int ringShotAngle;
					// 発射を10発分繰り返す。同時発射
					// 円形の弾幕の発射
					for (int i = 0; i < CHARGE_ENEMY_RING_BULLET_AMOUNT; i++)
					{
						// 36度毎に発射角度を変更するよう設定
						ringShotAngle = i * CHARGE_ENEMY_RING_SHOT_ANGLE;

						//mBulletManager->setBullet(
						//    (int)chargeEnemy->mX, (int)chargeEnemy->mY,
						//    ringShotAngle, ENEMY_NOMAL, true);
					}
					// 発射SEの再生
					PlaySoundMem(
					    Data::getInstance()->mEnemyShotSoundEffectHandle,
					    DX_PLAYTYPE_BACK, TRUE);
				}
				// 発射カウントが80を超えたら実行
				// プレイヤー狙いのミニ弾の発射
				else if (chargeEnemy->mShotCount >= CHARGE_ENEMY_AIM_SHOT_START)
				{
					// 5フレーム間隔でミニ弾を発射
					if (chargeEnemy->mShotCount %
					        CHARGE_ENEMY_AIM_SHOT_INTERVAL ==
					    0)
					{
						// 発射を3回繰り返す。同時に発射
						// 扇状の弾幕を発射
						for (int i = 0; i < CHARGE_ENEMY_AIM_BULLET_AMOUNT; i++)
						{
							// プレイヤー狙いの角度を計算
							double vectorTargetX =
							    (mPlayer->mX - chargeEnemy->mX);
							double vectorTargetY =
							    (mPlayer->mY - chargeEnemy->mY);
							double targetAngle =
							    atan2(vectorTargetY, vectorTargetX);

							double targetAngle_degPre =
							    targetAngle * 180.0f / PI;

							//mBulletManager->setBullet(
							//    (int)chargeEnemy->mX, (int)chargeEnemy->mY,
							//    targetAngle_degPre + 20 * i - 20,
							//    ENEMY_MINI_ORANGE, false);
						}
						// 発射SEの再生
						PlaySoundMem(
						    Data::getInstance()->mEnemyShotSoundEffectHandle,
						    DX_PLAYTYPE_BACK, TRUE);
					}
				}
				// カウントのリセット、170フレームで1巡
				if (chargeEnemy->mShotCount >= CHARGE_ENEMY_SHOT_RESET_FRAME)
				{
					chargeEnemy->mShotCount = 0;
				}
			}
		}
	}
}

/*
@brief	ボス敵の弾発射の管理を行う関数

@param[in]	bossEnemy* boss:ボス敵クラスのインスタンスポインタ

@return		なし

@note

- ボスのフラグ(FLG)がtrueで撃破フラグ(Defeat)がfalseの場合のみ、処理を実行する
-
bossEnemyクラスメンバのbossShootCountmini変数及びFrameCount変数でカウントし、処理を管理している
- 発射時に発射SEの再生をしている
- 発射パターンの切り替え時には、アイテムのドロップと爆破エフェクト表示を行う

-
HPによって発射を４パターンに分岐している。段々と激しい発射パターンに変化し最後は救済アイテムがないと突破は難しい

1. HP100%～75%、
    - ミニ弾の円状の弾幕を一定間隔で発射するのを繰り返している
    - 同時に12方向へミニ弾を発射

2. HP74%～50%、　
    - ミニ弾の円状の弾幕を一定間隔で発射するのを繰り返している
    - 同時に12方向へミニ弾を発射
    - 発射開始すると4連属でミニ弾の弾幕を発射
    - 発射間隔は前の発射パターンよりも短くなっている

3. HP49%～25%、　
    - ミニ弾と通常弾の2種類の弾を発射する
        - ミニ弾の円状の弾幕を一定間隔で発射するのを繰り返している
            - 同時に12方向へミニ弾を発射
            - 発射開始すると4連属でミニ弾の弾幕を発射
            - 発射間隔は前の発射パターンよりも短くなっている
        - 通常弾の円状の弾幕を発射するのを繰り返している
            - 発射された弾は徐々に減速
            - ミニ弾の弾幕よりも発射間隔は長い
            - 30方向へ同時に通常弾を発射

4. HP24%～0%,　　
    - ミニ弾と通常弾の2種類の弾を発射する
        - ミニ弾の円状の弾幕の発射を絶え間なく繰り返している
            - 一定間隔でミニ弾の弾幕の発射角度をずらしている
            - 同時に12方向へミニ弾を発射
        - 通常弾の円状の弾幕を発射するのを繰り返している
            - 発射された弾は徐々に減速
            - ミニ弾の弾幕よりも発射間隔は長い
            - 30方向へ同時に通常弾を発射
    - 三種の雑魚敵を召喚している
    - ボスが倒された場合、召喚した敵は初期状態へ戻している

@note　2重ループのfor分には変数ｊではなくkを使用している。iとjが見分けずらいので。３重の場合はlを使用

*/
void GameMainScene::shotBossEnemyBullet(BossEnemy *bossEnemy)
{
	if (bossEnemy->mIsActive && bossEnemy->mIsDefeat == false)
	{
		bossEnemy->mShotMiniBulletCount++; // ミニ弾の発射カウントの更新

		// HP100%～75%
		if (bossEnemy->mBossHitPointPercent <= BOSS_HP_PER_MAX &&
		    bossEnemy->mBossHitPointPercent > BOSS_HP_PER_PHASE1_END)
		{
			// 30Fでミニ弾を発射
			if (bossEnemy->mShotMiniBulletCount >=
			    BOSS_MINI_SHOT_INTERVAL_FRAME)
			{
				int miniBulletAngle;
				// 発射を12発分繰り返す。同時発射
				// 円形の弾幕
				for (int i = 0; i < BOSS_MINI_SHOT_AMOUNT; i++)
				{
					// 30毎に発射角度を変更
					miniBulletAngle = i * BOSS_MINI_SHOT_ANGLE_STEP;

					//mBulletManager->setBullet(
					//    (int)bossEnemy->mX, (int)bossEnemy->mY, miniBulletAngle,
					//    ENEMY_MINI_YELLOW, false);
				}
				// 発射カウントのリセット
				bossEnemy->mShotMiniBulletCount = 0;
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}
		}

		// HP75%
		else if (bossEnemy->mBossHitPointPercent == BOSS_HP_PER_PHASE1_END)
		{
			// 一度だけ処理を実行させる
			if (bossEnemy->mIsExecuteProcess[SECOND_EXECUTE] == FALSE)
			{
				// フラグの切り替え
				bossEnemy->mIsExecuteProcess[SECOND_EXECUTE] = TRUE;
				// アイテムドロップ
				dropItem((int)bossEnemy->mX, (int)bossEnemy->mY, OBJECT_EXP);
				dropItem((int)bossEnemy->mX + DROP_OFFSET_X, (int)bossEnemy->mY,
				         OBJECT_STAR);
				dropItem((int)bossEnemy->mX, (int)bossEnemy->mY, OBJECT_LIFE);
				// 行動切り替え時SEの再生
				bossEnemy->playSoundEffect();

				// 爆破エフェクト表示を4個分繰り返す
				for (int i = 0; i < BOSS_EXPLOSION_COUNT; i++)
				{
					// フラグのたっていないものを探し、1つだけsetexplosion関数実行
					for (int k = 0; k < MAX_BULLET_NUMBER; k++)
					{
						// 爆破エフェクトの表示
						// 重ならないようにループ回数によって位置を変更
						if (mExplosions[k]->setExplosion(
						        (int)bossEnemy->mX + BOSS_EXPLOSION_START_X +
						            i * BOSS_EXPLOSION_STEP_X,
						        (int)bossEnemy->mY + BOSS_EXPLOSION_OFFSET_Y))
						{
							break;
						}
					}
				}
				// 発射カウントのリセット
				bossEnemy->mShotBulletCount = 0;
				bossEnemy->mShotMiniBulletCount = 0;
			}
		}

		// HP74%～50%
		else if (bossEnemy->mBossHitPointPercent < BOSS_HP_PER_PHASE1_END &&
		         bossEnemy->mBossHitPointPercent > BOSS_HP_PER_PHASE2_END)
		{
			// 30フレーム間隔でミニ弾発射カウントをリセット
			if (bossEnemy->mShotMiniBulletCount > BOSS_MINI_BURST_RESET_FRAME)
			{
				bossEnemy->mShotMiniBulletCount = 0;
			}
			// ミニ弾発射カウントが15以内に4フレーム間隔で発射を実行(計4回
			if (bossEnemy->mShotMiniBulletCount <
			        BOSS_MINI_BURST_ACTIVE_FRAME &&
			    bossEnemy->mShotMiniBulletCount %
			            BOSS_MINI_BURST_INTERVAL_FRAME ==
			        0)
			{
				int miniBulletAngle;
				// 発射を12発分繰り返す。同時発射
				// 円形の弾幕
				for (int i = 0; i < BOSS_MINI_SHOT_AMOUNT; i++)
				{
					// 30度毎に発射角度を変更
					miniBulletAngle = i * BOSS_MINI_SHOT_ANGLE_STEP;

					//mBulletManager->setBullet(
					//    (int)bossEnemy->mX, (int)bossEnemy->mY, miniBulletAngle,
					//    ENEMY_MINI_YELLOW, false);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}
		}

		// HP50%
		else if (bossEnemy->mBossHitPointPercent == BOSS_HP_PER_PHASE2_END)
		{
			// 一度だけ処理を実行させる
			if (bossEnemy->mIsDropExperiencePoint == FALSE)
			{
				// アイテムのドロップ
				dropItem((int)bossEnemy->mX, (int)bossEnemy->mY, OBJECT_EXP);
				dropItem((int)bossEnemy->mX + DROP_OFFSET_X, (int)bossEnemy->mY,
				         OBJECT_STAR);
				dropItem((int)bossEnemy->mX, (int)bossEnemy->mY, OBJECT_LIFE);

				// フラグの切り替え
				bossEnemy->mIsDropExperiencePoint = TRUE;
				// 行動切り替え時SEの再生
				bossEnemy->playSoundEffect();

				// 爆破エフェクト表示を4個分繰り返す
				for (int i = 0; i < BOSS_EXPLOSION_COUNT; i++)
				{
					// フラグのたっていないものを探し、1つだけsetexplosion関数実行
					for (int k = 0; k < MAX_BULLET_NUMBER; k++)
					{
						// 爆破エフェクトの表示
						// 重ならないようにループ回数によって位置を変更
						if (mExplosions[k]->setExplosion(
						        (int)bossEnemy->mX + BOSS_EXPLOSION_START_X +
						            i * BOSS_EXPLOSION_STEP_X,
						        (int)bossEnemy->mY + BOSS_EXPLOSION_OFFSET_Y))
						{
							break;
						}
					}
				}
				// 発射カウントをリセット
				bossEnemy->mShotBulletCount = 0;
				bossEnemy->mShotMiniBulletCount = 0;
			}
		}

		// HP49%～25%
		else if (bossEnemy->mBossHitPointPercent < BOSS_HP_PER_PHASE2_END &&
		         bossEnemy->mBossHitPointPercent > BOSS_HP_PER_PHASE3_END)
		{
			// 30フレーム間隔でミニ弾発射カウントをリセット
			if (bossEnemy->mShotMiniBulletCount > BOSS_MINI_BURST_RESET_FRAME)
			{
				bossEnemy->mShotMiniBulletCount = 0;
			}

			// ミニ弾発射カウントが15以内に4フレーム間隔で発射を実行(計4回
			if (bossEnemy->mShotMiniBulletCount <
			        BOSS_MINI_BURST_ACTIVE_FRAME &&
			    bossEnemy->mShotMiniBulletCount %
			            BOSS_MINI_BURST_INTERVAL_FRAME ==
			        0)
			{
				int miniBulletAngle;

				// 発射を12発分繰り返す。同時発射
				// ミニ弾の円形の弾幕
				for (int i = 0; i < BOSS_MINI_SHOT_AMOUNT; i++)
				{
					// 30度毎に発射角度を変更
					miniBulletAngle = i * BOSS_MINI_SHOT_ANGLE_STEP;

					//mBulletManager->setBullet(
					//    (int)bossEnemy->mX, (int)bossEnemy->mY, miniBulletAngle,
					//    ENEMY_MINI_YELLOW, false);

				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}
			// 弾発射カウント更新
			bossEnemy->mShotBulletCount++;
			// 120フレーム間隔で発射を実行
			if (bossEnemy->mShotBulletCount >= BOSS_NOMAL_SHOT_INTERVAL_FRAME)
			{
				int ringBulletAngle;
				// 発射を30発分繰り返す。同時発射
				// 通常弾の円形の弾幕
				for (int i = 0; i < BOSS_NOMAL_SHOT_AMOUNT; i++)
				{
					// 12度毎に角度を変更
					ringBulletAngle = i * BOSS_NOMAL_SHOT_ANGLE_STEP;

					//mBulletManager->setBullet(
					//    (int)bossEnemy->mX, (int)bossEnemy->mY, ringBulletAngle,
					//    ENEMY_NOMAL, true);

				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
				bossEnemy->mShotBulletCount = 0; // 発射カウントのリセット
			}
		}

		// HP25%
		else if (bossEnemy->mBossHitPointPercent == BOSS_HP_PER_PHASE3_END)
		{
			// 一度だけ実行させる処理
			if (bossEnemy->mIsExecuteProcess[THIRD_EXECUTE] == FALSE)
			{
				// アイテムのドロップ
				dropItem((int)bossEnemy->mX, (int)bossEnemy->mY, OBJECT_EXP);
				dropItem((int)bossEnemy->mX + DROP_OFFSET_X, (int)bossEnemy->mY,
				         OBJECT_STAR);
				dropItem((int)bossEnemy->mX, (int)bossEnemy->mY, OBJECT_LIFE);

				// 行動切り替え時SEの再生
				bossEnemy->playSoundEffect();

				// 爆破エフェクト表示を4個分繰り返す
				for (int i = 0; i < BOSS_EXPLOSION_COUNT; i++)
				{
					// フラグのたっていないものを探し、1つだけsetexplosion関数実行
					for (int k = 0; k < MAX_BULLET_NUMBER; k++)
					{
						// 爆破エフェクトの表示
						// 重ならないようにループ回数によって位置を変更
						if (mExplosions[k]->setExplosion(
						        (int)bossEnemy->mX + BOSS_EXPLOSION_START_X +
						            i * BOSS_EXPLOSION_STEP_X,
						        (int)bossEnemy->mY + BOSS_EXPLOSION_OFFSET_Y))
						{
							break;
						}
					}
				}
				// カウントのリセット、フラグの切り替え
				bossEnemy->mIsExecuteProcess[THIRD_EXECUTE] = TRUE;
				bossEnemy->mShotBulletCount = 0;
				bossEnemy->mShotMiniBulletCount = 0;
			}
		}

		// HP24%～0%
		else if (bossEnemy->mBossHitPointPercent < BOSS_HP_PER_PHASE3_END &&
		         bossEnemy->mBossHitPointPercent > BOSS_HP_PER_DEFEAT)
		{
			// 弾発射カウントの更新
			bossEnemy->mShotBulletCount++;
			// 30フレーム間隔でミニ弾発射カウントをリセット
			if (bossEnemy->mShotMiniBulletCount > BOSS_MINI_BURST_RESET_FRAME)
			{
				bossEnemy->mShotMiniBulletCount = 0;
			}
			// 15F間隔で発射角度を変更
			// 4フレーム間隔で発射
			if (bossEnemy->mShotMiniBulletCount <
			        BOSS_MINI_BURST_ACTIVE_FRAME &&
			    bossEnemy->mShotMiniBulletCount %
			            BOSS_MINI_BURST_INTERVAL_FRAME ==
			        0)
			{
				int miniBulletAngle;
				// 発射を12発分繰り返す。同時発射
				// ミニ弾の円形の弾幕
				for (int i = 0; i < BOSS_MINI_SHOT_AMOUNT; i++)
				{
					// 30度毎に発射角度を変更
					miniBulletAngle = i * BOSS_MINI_SHOT_ANGLE_STEP;

					//mBulletManager->setBullet(
					//    (int)bossEnemy->mX, (int)bossEnemy->mY, miniBulletAngle,
					//    ENEMY_MINI_YELLOW, false);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}
			// 15フレーム毎に発射角度をずらす
			// 4フレーム間隔で発射
			else if (bossEnemy->mShotMiniBulletCount %
			             BOSS_MINI_BURST_INTERVAL_FRAME ==
			         0)
			{
				int miniBulletAngle;
				// 発射を12発分繰り返す。同時発射
				// ミニ弾の円形の弾幕
				for (int i = 0; i < BOSS_MINI_SHOT_AMOUNT; i++)
				{
					// 30度毎に発射角度を変更に加えて20度ずらす
					miniBulletAngle = i * BOSS_MINI_SHOT_ANGLE_STEP +
					                  BOSS_MINI_BURST_OFFSET_ANGLE;
					//mBulletManager->setBullet(
					//    (int)bossEnemy->mX, (int)bossEnemy->mY, miniBulletAngle,
					//    ENEMY_MINI_YELLOW, false);

				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
			}

			// 120フレーム間隔で発射を実行
			if (bossEnemy->mShotBulletCount >= BOSS_NOMAL_SHOT_INTERVAL_FRAME)
			{
				int ringBulletAngle;
				// 発射を30発分繰り返す。同時発射
				// 通常弾の円形の弾幕
				for (int i = 0; i < BOSS_NOMAL_SHOT_AMOUNT; i++)
				{
					// 12度毎に角度を変更
					ringBulletAngle = i * BOSS_NOMAL_SHOT_ANGLE_STEP;
					//mBulletManager->setBullet(
					//    (int)bossEnemy->mX, (int)bossEnemy->mY, ringBulletAngle,
					//    ENEMY_NOMAL, true);
				}
				// 発射SEの再生
				PlaySoundMem(Data::getInstance()->mEnemyShotSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
				bossEnemy->mShotBulletCount = 0; // 発射カウントのリセット
			}
			// 雑魚敵の召喚
			if (bossEnemy
			        ->mIsSummonEnemy) // 召喚フラグがtrueの場合(ボス敵の移動処理で切り替え)
			{
				// 修正、マジックナンバー
				// 召喚する敵の初期化
				mMinionEnemyManager->mChargeEnemies[SUMMON_LEFT]->start();
				mMinionEnemyManager->mEnemies[SUMMON_LEFT]->start();
				mMinionEnemyManager->mTraceEnemies[SUMMON_LEFT]->start();

				mMinionEnemyManager->mChargeEnemies[SUMMON_RIGHT]->start();
				mMinionEnemyManager->mEnemies[SUMMON_RIGHT]->start();
				mMinionEnemyManager->mTraceEnemies[SUMMON_RIGHT]->start();

				// 左側召喚
				mMinionEnemyManager->mChargeEnemies[SUMMON_LEFT]->pop(
				    SUMMON_LEFT_CHARGE_ENEMY_X,
				                                 SUMMON_ENEMY_Y, SUMMON_LEFT,
				                                 true);
				mMinionEnemyManager->mEnemies[SUMMON_LEFT]->pop(
				    SUMMON_LEFT_NOMAL_ENEMY_X,
				                           SUMMON_ENEMY_Y, SUMMON_LEFT, true);
				mMinionEnemyManager->mTraceEnemies[SUMMON_LEFT]->pop(
				    SUMMON_LEFT_TRACE_ENEMY_X,
				                                SUMMON_ENEMY_Y, SUMMON_LEFT,
				                                true);

				// 右側召喚
				mMinionEnemyManager->mChargeEnemies[SUMMON_RIGHT]->pop(
				    SUMMON_RIGHT_CHARGE_ENEMY_X,
				                                  SUMMON_ENEMY_Y, SUMMON_RIGHT,
				                                  true);
				mMinionEnemyManager->mEnemies[SUMMON_RIGHT]->pop(
				    SUMMON_RIGHT_NOMAL_ENEMY_X,
				                            SUMMON_ENEMY_Y, SUMMON_RIGHT, true);
				mMinionEnemyManager->mTraceEnemies[SUMMON_RIGHT]->pop(
				    SUMMON_RIGHT_TRACE_ENEMY_X,
				                                 SUMMON_ENEMY_Y, SUMMON_RIGHT,
				                                 true);
			}
		}

		// HP0%
		else if (bossEnemy->mBossHitPointPercent == BOSS_HP_PER_DEFEAT)
		{
			// 修正、マジックナンバー
			// 召喚した敵の初期化
			mMinionEnemyManager->mChargeEnemies[SUMMON_LEFT]->start();
			mMinionEnemyManager->mEnemies[SUMMON_LEFT]->start();
			mMinionEnemyManager->mTraceEnemies[SUMMON_LEFT]->start();

			mMinionEnemyManager->mChargeEnemies[SUMMON_RIGHT]->start();
			mMinionEnemyManager->mEnemies[SUMMON_RIGHT]->start();
			mMinionEnemyManager->mTraceEnemies[SUMMON_RIGHT]->start();
		}
	}
}

/*
@brief	プレイヤーの弾発射を管理をする関数

@param[in]	int playerX		:プレイヤーのX座標
@param[in]	int playerY		:プレイヤーのY座標
@param[in]	int Power		:プレイヤーのショットパワー

@return		なし

@note

      - スペースキーを押しっぱなしの場合、弾を連続で発射している
      - 発射パターン変化時に強化SEの再生と強化エフェクトの表示を行う
      - 次の変数でカウントを行い、処理を管理している
        - shootdileycount			通常弾の発射間隔を管理
        - shootmisailedileycount	ミサイル弾発射間隔を管理
        - Scount　　				スペシャル弾の発射間隔を管理
        - shotcount　				共通の発射間隔を管理
      　
      - ショットパワーに応じて発射パターンが強化されている

        1. ショットパワー1500以下
           - 一定間隔でプレイヤー通常弾の発射を繰り返す

        2. ショットパワー1500以上
           -  発射間隔を短くして一定間隔でプレイヤー通常弾の発射を繰り返す

        3. ショットパワー3000以上
           -  間隔を短くして一定間隔でプレイヤー通常弾の発射を繰り返す
           - ミサイル弾の発射を追加
             -左右同時に発射を繰り返している。

        4. ショットパワー6000以上、　
           - 間隔を短くして一定間隔でプレイヤー通常弾の発射を繰り返す
           - ミサイル弾の発射を追加
             - 左右同時に発射を繰り返している
           - スペシャル弾の追加
             - スペシャル弾の扇状の弾幕を一定間隔で繰り返し発射
             - 5方向にスペシャル弾を同時に発射
             - 5連続で弾幕を発射

@note　2重ループのfor分には変数ｊではなくkを使用している。iとjが見分けずらいので。３重の場合はlを使用

*/
void GameMainScene::shotPlayerBullet(int playerX, int playerY, int shotPower)
{
	// 発射カウントの更新
	mPlayerShotCount++;
	// 5フレーム間隔で処理を実行
	if (mPlayerShotCount > PLAYER_NOMAL_SHOT_INTERVAL)
	{
		mPlayerShotCount = 0; // 発射カウントのリセット
		// スペースキーを押されると発射
		if (checkPushkey(KEY_INPUT_SPACE))
		{
			mNomalShotIntervalCount++; // 発射間隔カウントの更新
			// 発射間隔カウントの2カウント間隔で実行(実質10フレーム
			if (mNomalShotIntervalCount % mNomalShotInterval == 0)
			{
				mNomalShotIntervalCount = 0;
				for (int i = 0; i < PLAYER_MISSILE_AMOUNT; i++)
				{

					mBulletManager->setBullet(playerX - 18 + i * 36,
					                          playerY - 5, -90,
					                          BULLET_TYPE::PLAYER_NOMAL, false);
				}
			}
			// shootPowarが1500超えたら発射間隔を短くする
			if (shotPower >= SHOT_POWER_GRADE1 &&
			    mNomalShotInterval == PLAYER_NORMAL_SHOT_SPEED)
			{
				// 強化SEの再生
				PlaySoundMem(Data::getInstance()->mPowerUpSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
				// フラグのたっていないものを探し、1つだけsetef関数を実行
				for (int i = 0; i < MAX_BULLET_NUMBER; i++)
				{
					// パワーアップエフェクトの表示
					if (mEffects[i]->setEffect(&(mPlayer->mX), &(mPlayer->mY),
					                           POWERUP_EF))
					{
						break;
					}
				}

				mNomalShotInterval =
				    PLAYER_UPGRADE_SHOT_SPEED; // 発射間隔カウントの1カウント間隔で実行（実質5フレーム）
			}

			// shootPowarが３０００超えたらミサイル弾追加
			// ミサイル発射フラグで1度だけ実行
			if (shotPower >= SHOT_POWER_GRADE2 &&
			    mPlayer->mIsActiveMissileShot == false)
			{
				// 強化SEの再生
				PlaySoundMem(Data::getInstance()->mPowerUpSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
				// フラグのたっていないものを探し、1つだけsetef関数を実行
				for (int i = 0; i < MAX_BULLET_NUMBER; i++)
				{
					// パワーアップエフェクトの表示
					if (mEffects[i]->setEffect(&(mPlayer->mX), &(mPlayer->mY),
					                           POWERUP_EF))
					{
						break;
					}
				}
				// ミサイル弾発射フラグをオン
				mPlayer->mIsActiveMissileShot = true;
			}
			// ミサイル弾発射フラグがオンの場合
			if (mPlayer->mIsActiveMissileShot)
			{
				// ミサイル弾発射カウントの更新
				mMissileShotIntervalCount++;
				if (mMissileShotIntervalCount % PLAYER_MISSILE_SHOT_INTERVAL ==
				    0)
				{
					mMissileShotIntervalCount =
					    0; // ミサイル弾発射カウントのリセット

					// 2発分発射を繰り返す
					// 左斜め上、右斜め上方向に向けて発射
					for (int i = 0; i < PLAYER_MISSILE_AMOUNT; i++)
					{

						mBulletManager->setBullet(
						    playerX - 18 + i * 36, playerY - 5,
						    i * PLAYER_MISSILE_ANGLE_STEP +
						        PLAYER_MISSILE_START_ANGLE + 180,
						    BULLET_TYPE::PLAYER_MISSILE, false);
					}
				}

				// カウントのオーバフロー防止
				if (mMissileShotIntervalCount >=
				    PLAYER_MISSILE_SHOT_COUNTER_RESET)
				{
					mMissileShotIntervalCount = 0;
				}
			}
			if (shotPower == SHOT_POWER_GRADE3) // 一度だけ処理を実行させる
			{
				// 強化SEの再生
				PlaySoundMem(Data::getInstance()->mPowerUpSoundEffectHandle,
				             DX_PLAYTYPE_BACK, TRUE);
				// フラグのたっていないものを探し、1つだけsetef関数を実行
				for (int i = 0; i < MAX_BULLET_NUMBER; i++)
				{
					// パワーアップエフェクトの表示
					if (mEffects[i]->setEffect(&(mPlayer->mX), &(mPlayer->mY),
					                           POWERUP_EF))
					{
						break;
					}
				}
			}

			// shootPowarが６０００超えたらスペシャル使用可能
			if (shotPower >= SHOT_POWER_GRADE3)
			{
				// スペシャル弾発射カウントの更新
				mSpecialShotCount++;

				// 5カウントまで発射を繰り返す
				// 発射間隔は実質25フレーム
				if (mSpecialShotCount < PLAYER_SPECIAL_SHOT_ACTIVE_COUNT)
				{
					// 5発分発射を繰り返す。同時発射
					// スペシャル弾の扇状の弾幕
					for (int i = 0; i < PLAYER_SPECIAL_AMOUNT; i++)
					{
						mBulletManager->setBullet(
						    playerX, playerY, i * 30 - 90 - 60,
						    BULLET_TYPE::PLAYER_SPECIAL, false);
					}
				}
				// 10カウントでスペシャル弾発射カウントをリセット(実質50フレーム)
				if (mSpecialShotCount > PLAYER_SPECIAL_SHOT_COUNTER_RESET)
				{
					mSpecialShotCount = 0;
				}
			}
		}
	}
}

/*
@brief	アイテムのドロップを管理する関数

@param[in]	int x			:出現させるのX座標
@param[in]	int y			:出現させるのY座標
@param[in]	int itemtype	:出現させるアイテムの種類

@return		なし

@note

      - アイテムに応じて処理を分岐している

        1. 経験値アイテム
           - 敵から飛び出すように角度を設定している。
           - 6つ同時にドロップさせている。

        2. 回復アイテム
           - 10パーセントの確率でドロップするようにしている。

        3. 無敵アイテム
           - 10パーセントの確率でドロップするようにしている。

      -
雑魚敵を撃破した時場合及びボスの発射と行動パターンの切り替えする場合に使用している。
@note　2重ループのfor分には変数ｊではなくkを使用している。iとjが見分けずらいので。３重の場合はlを使用
*/
void GameMainScene::dropItem(int itemX, int itemY, int itemType)
{
	// 乱数用変数
	int randomNumber = 0;

	switch (itemType) // アイテム種類で分岐
	{
	case OBJECT_EXP: // 経験値アイテム

		// 6つの経験値アイテムを出現させる
		for (int i = 0; i < DROP_EXP_ITEM_AMOUNT; i++)
		{
			// 60度ごとに角度を変更
			int expItemAngle = (i + DROP_EXP_START_INDEX) * DROP_EXP_ANGLE_STEP;

			// フラグのたっていないものを探し、1つだけset関数を実行
			for (int k = 0; k < MAX_BULLET_NUMBER; k++)
			{
				// 経験値アイテムの設置
				if (mItemObjects[k]->setItemObject(itemX, itemY, expItemAngle,
				                                   OBJECT_EXP, &(mPlayer->mX),
				                                   &(mPlayer->mY)))
				{
					break;
				}
			}
		}

		break;

	case OBJECT_LIFE: // 回復アイテム

		// 乱数の取得
		randomNumber = GetRand(DROP_RATE_RANDOM_MAX);

		// 10％の確率で実行
		if (randomNumber <= OBJECT_LIFE_DROP_PAESENT)
		{
			// フラグのたっていないものを探し、1つだけset関数を実行
			for (int i = 0; i < MAX_BULLET_NUMBER; i++)
			{
				// 回復アイテムの設置
				if (mItemObjects[i]->setItemObject(itemX, itemY, 0, OBJECT_LIFE,
				                                   &(mPlayer->mX),
				                                   &(mPlayer->mY)))
				{
					break;
				}
			}
		}

		break;

	case OBJECT_STAR: // 無敵アイテム

		// 乱数の取得
		randomNumber = GetRand(DROP_RATE_RANDOM_MAX);
		// 30％野確率で実行
		if (randomNumber <= OBJECT_STAR_DROP_PAESENT)
		{
			// フラグのたっていないものを探し、1つだけset関数を実行
			for (int i = 0; i < MAX_BULLET_NUMBER; i++)
			{
				// 無敵アイテムを設置
				if (mItemObjects[i]->setItemObject(itemX, itemY, 0, OBJECT_STAR,
				                                   &(mPlayer->mX),
				                                   &(mPlayer->mY)))
				{
					break;
				}
			}
		}

		break;

	default:
		break;
	}
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
			// フラグのたっていないものを探し、1つだけsetexplosion関数を実行
			for (int i = 0; i < MAX_BULLET_NUMBER; i++)
			{
				// ボス画像上のランダムな位置を計算
				int randomExplositionPositionX =
				    (int)mBoss->mX + BOSS_DEFEAT_EXPLOSION_OFFSET_X +
				    GetRand(BOSS_DEFEAT_EXPLOSION_RANDOM_RANGE) *
				        BOSS_DEFEAT_EXPLOSION_STEP;
				int randomExplositionPositionY =
				    (int)mBoss->mY + BOSS_DEFEAT_EXPLOSION_OFFSET_Y +
				    GetRand(BOSS_DEFEAT_EXPLOSION_RANDOM_RANGE) *
				        BOSS_DEFEAT_EXPLOSION_STEP;
				// 爆破エフェクトの表示
				if (mExplosions[i]->setExplosion(randomExplositionPositionX,
				                                 randomExplositionPositionY))
				{
					break;
				}
			}
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
