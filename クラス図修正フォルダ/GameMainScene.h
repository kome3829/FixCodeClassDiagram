#pragma once
#include "basicScene.h"
// --- フェード演出 ---
#define FADE_OUT_SPEED             (3)
#define REDBG_FADEOUT_COUNT_MAX         (3)

// --- ボスHP ---
#define TEST_BOSS_HP               (1500)

// --- テスト用プレイヤー攻撃力 ---
#define TEST_PLAYER_POWER          (6100)

// --- 背景動画ループ ---
#define MOVIE_LOOP_FRAME        (600)
#define MOVIE_LOOP_START        (0)

// --- アイテムドロップ位置補正 ---
#define DROP_OFFSET_X              (20)

// --- UIエリア ---
#define UI_BOX_TOP                (460)
#define UI_BOX_BOTTOM             (480)
#define UI_SCORE_X                (10)
#define UI_SHOT_POWER_X           (350)
#define UI_TIME_X                 (200)

// --- ボスHP表示 ---
#define BOSS_HP_TEXT_X            (10)
#define BOSS_HP_TEXT_Y            (20)
#define BOSS_HP_BAR_LEFT_X        (0)
#define BOSS_HP_BAR_TOP_Y         (0)
#define BOSS_HP_BAR_RIGHT_X       (320)
#define BOSS_HP_BAR_BOTTOM_Y      (14)
#define BOSS_HP_INNER_LEFT_X      (2)
#define BOSS_HP_INNER_TOP_Y       (2)
#define BOSS_HP_INNER_BOTTOM_Y    (12)

// --- フォントサイズ ---
#define UI_FONT_SIZE              (16)

// --- 色 ---
#define COLOR_BLACK               (0x000000)
#define COLOR_WHITE               (0xffffff)
#define COLOR_GREEN               (0x00ff00)
#define COLOR_RED                 (0xff0000)
// --- 敵出現間隔 ---
#define ENEMY_POP_DIVISOR     (3)
// --- チャージ敵出現タイミング ---
#define CHARGE_ENEMY_POP_INTERVAL_FRAME      (30)
#define CHARGE_ENEMY_POP_END_COUNT          (ENEMY_POP_FRAME + CHARGE_ENEMY_POP_INTERVAL_FRAME * 6 - 10)

// --- 出現座標 ---
#define CHARGE_ENEMY_POP_LEFT_X            (-20)
#define CHARGE_ENEMY_POP_RIGHT_X           (660)
#define CHARGE_ENEMY_POP_Y                 (0)
// --- 追尾敵弾発射タイミング ---
#define TRACE_ENEMY_SHOT_START_FRAME       (60)
#define TRACE_ENEMY_SHOT_INTERVAL_FRAME    (10)
#define TRACE_ENEMY_SHOT_END_FRAME        (100)
#define TRACE_ENEMY_SHOT_RESET_FRAME      (180)

// --- 4方向弾 ---
#define TRACE_ENEMY_SHOT_WAY_COUNT          (4)
#define TRACE_ENEMY_SHOT_BASE_ANGLE        (90)
// --- チャージ敵 射撃タイミング ---
#define CHARGE_EF_INTERVAL     (40)
#define CHARGE_EF_END          (120)
#define CHARGE_ENEMY_SHOT_START_FRAME       (120)
#define CHARGE_ENEMY_SHOT_INTERVAL_FRAME    (10)
#define CHARGE_ENEMY_SHOT_RESET_FRAME       (170)

// --- ボスステージ用 MoveCount ---
#define CHARGE_ENEMY_BOSS_CHARGE_END           (240)
#define CHARGE_ENEMY_BOSS_WARP_EF_FRAME        (304)
#define CHARGE_ENEMY_BOSS_ATTACK_START         (331)
#define CHARGE_ENEMY_BOSS_ITEM_DROP_FRAME      (345)

// --- ボスステージ射撃 ---
#define CHARGE_ENEMY_RING_SHOT_INTERVAL         (50)
#define CHARGE_ENEMY_RING_BULLET_AMOUNT         (10)
#define CHARGE_ENEMY_RING_SHOT_ANGLE            (36)

#define CHARGE_ENEMY_AIM_SHOT_START             (80)
#define CHARGE_ENEMY_AIM_SHOT_INTERVAL          (5)
#define CHARGE_ENEMY_AIM_BULLET_AMOUNT          (3)
#define CHARGE_ENEMY_AIM_SHOT_SPREAD            (0.2)

// --- 救済アイテム出現位置 ---
#define ITEM_DROP_LEFT_X              (100)
#define ITEM_DROP_RIGHT_X             (500)
#define ITEM_DROP_Y                   (240)

// --- ボス弾幕 ---

// ミニ弾 共通
#define BOSS_MINI_SHOT_INTERVAL_FRAME       (30)
#define BOSS_MINI_SHOT_AMOUNT               (12)
#define BOSS_MINI_SHOT_ANGLE_STEP           (30)

// 連射ミニ弾
#define BOSS_MINI_BURST_RESET_FRAME         (30)
#define BOSS_MINI_BURST_ACTIVE_FRAME        (15)
#define BOSS_MINI_BURST_INTERVAL_FRAME      (4)
#define BOSS_MINI_BURST_OFFSET_ANGLE        (20)

// 通常弾
#define BOSS_NOMAL_SHOT_INTERVAL_FRAME      (120)
#define BOSS_NOMAL_SHOT_AMOUNT              (30)
#define BOSS_NOMAL_SHOT_ANGLE_STEP          (12)

// 爆発演出
#define BOSS_EXPLOSION_COUNT        (4)
#define BOSS_EXPLOSION_START_X      (-30)
#define BOSS_EXPLOSION_STEP_X       (40)
#define BOSS_EXPLOSION_OFFSET_Y     (40)

// 雑魚召喚位置
#define SUMMON_LEFT_CHARGE_ENEMY_X            (30)
#define SUMMON_LEFT_NOMAL_ENEMY_X             (10)
#define SUMMON_LEFT_TRACE_ENEMY_X             (50)

#define SUMMON_RIGHT_CHARGE_ENEMY_X           (610)
#define SUMMON_RIGHT_NOMAL_ENEMY_X            (580)
#define SUMMON_RIGHT_TRACE_ENEMY_X           (630)

#define SUMMON_ENEMY_Y                       (0)

// 発射管理
#define PLAYER_NOMAL_SHOT_INTERVAL          (5)

// 通常弾
#define PLAYER_NORMAL_SHOT_SPEED       (2)
#define PLAYER_UPGRADE_SHOT_SPEED       (1)

// ミサイル弾
#define PLAYER_MISSILE_SHOT_INTERVAL  (3)//変数に代入から直接記述に変更
#define PLAYER_MISSILE_AMOUNT            (2)
#define PLAYER_MISSILE_START_ANGLE          (70)
#define PLAYER_MISSILE_ANGLE_STEP           (40)
#define PLAYER_MISSILE_SHOT_COUNTER_RESET      (1024)

// スペシャル弾
#define PLAYER_SPECIAL_AMOUNT            (5)
#define PLAYER_SPECIAL_SHOT_ACTIVE_COUNT         (5)
#define PLAYER_SPECIAL_SHOT_COUNTER_RESET       (10)
// EXPアイテム拡散
#define DROP_EXP_ITEM_AMOUNT            (6)
#define DROP_EXP_ANGLE_STEP           (60)
#define DROP_EXP_START_INDEX           (1)

// 乱数
#define DROP_RATE_RANDOM_MAX         (100)
// ボス撃破演出
#define DEFEAT_EXPLOSION_INTERVAL_FRAME     (15)

// 爆発位置ランダム
#define BOSS_DEFEAT_EXPLOSION_OFFSET_X         (-30)
#define BOSS_DEFEAT_EXPLOSION_OFFSET_Y         (-30)
#define BOSS_DEFEAT_EXPLOSION_RANDOM_RANGE       (5)
#define BOSS_DEFEAT_EXPLOSION_STEP             (20)

// 時間変換
#define TIME_MILLISECOND_TO_SECOND           (1000)
#define TIME_SECOND_PER_MINUTE                (60)

// プレイヤー死亡後リザルト遷移
#define PLAYER_DEFEAT_RESULT_WAIT_FRAME      (100)

enum STAGE//ステージパターン　
{
    NOMAL_STAGE,
    BOSS_STAGE,
    CLEAR_STAGE,
    TEST_STAGE,
};

enum ENEMY_POP_PATTERN//雑魚敵出現パターン　
{
    NOMAL_ENEMY,
    TRACE_ENEMY,
    CHARGE_ENEMY,
};



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
    //キャラ
    Player* mPlayer;                                      //プレイヤー
    Enemy* mEnemies[MAX_ENEMY_COUNT];                     //雑魚敵
    ChargeEnemy* mChargeEnemies[MAX_ENEMY_COUNT];         //チャージ攻撃敵
    TraceEnemy* mTraceEnemies[MAX_ENEMY_COUNT];           //追従敵
    BossEnemy* mBoss;                                      //ボス敵
    //弾
    EnemyBullet* mEnemyBullets[MAX_BULLET_NUMBER];         //敵通常弾
    EnemyMiniBullet* mEnemyMiniBullets[MAX_BULLET_NUMBER]; //敵ミニ弾
    Bullet* mBullets[MAX_BULLET_NUMBER];                   //プレイヤー弾
    MissileBullet* mMissileBullets[MAX_BULLET_NUMBER];     //プレイヤーミサイル弾
    SpecialBullet* mSpecialBullets[MAX_BULLET_NUMBER];     //プレイヤースペシャル弾
    HomingBullet* mHomingBullets[MAX_BULLET_NUMBER];       //プレイヤーホーミング弾
    //アイテム
    Object* mItemObjects[MAX_BULLET_NUMBER];                   //アイテム、オブジェクト
    //エフェクト
    Explosion* mExplosions[MAX_BULLET_NUMBER];             //爆発エフェクト
    Effect* mEffects[MAX_BULLET_NUMBER];                   //強化エフェクト

    //----------------------

    //---雑魚敵に関わる変数---

    int mEnemyNumber;           //雑魚敵の数
    int mEnemyPopCount;         //雑魚敵の出現カウント

    int mTraceEnemyNumber;      //追従敵数
    int mTraceEnemyPopCount;    //追従敵出現カウント
    
    int mChargeEnemyNumber;     //チャージ攻撃敵数
    int mChargeEnemyPopCount;   //チャージ攻撃敵出現カウント
    
    ENEMY_POP_PATTERN mEnemyPopPattern; //ノーマルステージの雑魚パターン　

    //------------------------
    
    //---ボス敵の演出に関わる変数---

    int mRedBGFadeOutCount;         //ボス出現演出回数カウント
    int mBossExploEFCount;          //爆発エフェクト再生回数カウント
    int mBossExploEFIntervalCount;  //爆発エフェクト再生間隔カウント
    bool mIsBossAlert;              //ボスアラート演出+ダメージ演出判定　赤背景
    int mRedBGAlpha;                //赤背景　透過度

    //------------------------------
    
    //---プレイヤーの発射に関する変数---
    int mPlayerShotCount;          //プレイヤー発射カウント
    int mNomalShotIntervalCount;   //通常弾発射間隔カウント
    int mNomalShotInterval;        //通常弾発射間隔
    int mMissileShotIntervalCount; //ミサイル発射間隔カウント
    int mSpecialShotCount;         //スペシャル弾発射カウント
    //---------------------------------
    
    //---ゲーム進行に関わる変数---
    int mScore;                 //スコア
    int mStartTime;             //開始時間
    int mTime;                  //経過時間
    int mMovieCount;            //動画更新カウント
    int mResultSwichDelayCount; //リザルトシーン以降カウント
    STAGE mStageType;           //ステージパターン

    //----------------------------
    
    //---敵に関わる関数---

    bool checkAllEnemyDefeat(); //雑魚敵が全部死んだか
    bool checkAllTraceEnemyDefeat();//全部死んだか
    bool checkAllChargeEnemyDefeat();//全部死んだか
    /// <summary>
    ///  <para>ENEMYPOP(９０) フレームで雑魚敵を出現</para> 
    ///  <para>出現場所　画面上部２か所　（POPLEFT、0）or (POPRIGHT, 0）</para>
    /// </summary>
    void popEnemy();
    /// <summary>
    ///  <para>ENEMYPOP(９０) フレームで追従敵を出現</para>
    /// <para> 出現場所　画面上部２か所　（POPLEFT、0）or (POPRIGHT, 0）</para>
    /// </summary>
    void popTraceEnemy();

    /// <summary>
    ///  <para>ENEMYPOP(９０) フレームでチャージ攻撃敵を出現</para>
    /// <para> 出現場所　画面上部２か所　（POPLEFT、0）or (POPRIGHT, 0）</para>
    /// </summary>
    void popChargeEnemy();

    //--------------------


    //---弾発射に関する関数---

    /// <summary>
    /// プレイヤー弾発射関数
    /// </summary>
    /// <param name="playerX=プレイヤーのX座標"></param>
    /// <param name="playerY=プレイヤーのY座標"></param>
    /// <param name="shotPower=ショットパワー"></param>
    void shotPlayerBullet(int playerX, int playerY, int shotPower);
    
    /// <summary>
    /// 敵弾発射関数
    /// </summary>
    /// <param name="enemy=Enemyクラスのポインタ"></param>
    void shotEnemyBullet(Enemy* enemy);

    /// <summary>
    /// 追従敵弾発射関数
    /// </summary>
    /// <param name="traceEnemy=TraceEnemyクラスのポインタ"></param>
    void shotTraceEnemyBullet(TraceEnemy* traceEnemy);

    /// <summary>
    /// チャージ敵弾発射関数
    /// </summary>
    /// <param name="chargeEnemy=ChargeEnemyクラスのポインタ"></param>
    void shotChargeEnemyBullet(ChargeEnemy* chargeEnemy);

    /// <summary>
    /// ボス敵弾発射関数
    /// </summary>
    /// <param name="bossEnemy=BossEnemyクラスのポインタ"></param>
    void shotBossEnemyBullet(BossEnemy* bossEnemy);

    //------------------------

    //---その他の関数---

    /// <summary>
    /// アイテムドロップ関数
    /// </summary>
    /// <param name="x=ドロップのX座標"></param>
    /// <param name="y=ドロップのY座標"></param>
    /// <param name="itemtype=アイテムの種類"></param>
    /// 
    void dropItem(int itemX, int itemY, int itemType);

    void playBossDefeatExplosion();//ボス撃破時爆破演出
    int checkPlayerDefeat();//負け判定

    //------------------

};

