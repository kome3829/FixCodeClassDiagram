#ifndef __BASICSCENE_H__
#define __BASICSCENE_H__

#include "DxLib.h"
#include"myLib/MyLib.h"
#include"myLib/key.h"
#include "math.h"
#include "define.h"
#include "Deta.h"
#include "Player.h"
#include "Enemy.h"
#include"Traceenemy.h"
#include"BossEnemy.h"
#include"ChargeEnemy.h"
#include"Bullet.h"
#include "Effect.h"

class BasicScene
{
public:
	/*
	@brief	コンストラクタ

	@param[in]	なし
	@return		なし

	@note	記載なし。クラス構造を作るための型として作成したので、定義のみ
	@note	クラスで定義した変数の初期設定などを行うために使用する
	@note	インスタンス生成時に自動で実行される
　　*/
	BasicScene() {};

	/*
	@brief	デストラクタ

	@param[in]	なし
	@return		なし

	@note	記載なし。クラス構造を作るための型として作成したので、定義のみ
	@note	クラス内で自動で解放されないメモリの開放を行う
	@note	newで宣言したものがあればこの関数内でdeleteを行う
	@note	※仮想化を行う必要がある。子クラスのデストラクタが呼び出されなくなる場合が出てくる
	@note	インスタンスの寿命が終わるときに自動で実行されるう
　　*/
	~BasicScene() {};

	/*
	@brief	メインループで実行する更新処理を行う関数

	@param[in]	なし
	@return     シーンナンバー  :int
	@note		タイトルシーン時/    SCENE_TITLE　  0
	@note		ゲームメインシーン時/SCENE_GAMEMAIN 1
	@note		リザルトシーン時/    SCENE_RESULT   2
				
	@note	記載なし。クラス構造を作るための型として作成したので、宣言のみ
	@note	シーン内の各オブジェクトの移動や当たり判定などの更新処理をこの関数へまとめる
	@note	このクラスを実際に使用する事がないので仮想化している
　　*/
	virtual int action() = 0;

	/*
	@brief	描画ループで実行する描画処理を行う関数

	@param[in]	なし

	@return		なし

	@note	記載なし。クラス構造を作るための型として作成したので、宣言のみ
	@note	シーン内の各オブジェクトの表示などの処理をこの関数へまとめる
	@note	このクラスを実際に使用する事がないので仮想化している
　　*/
	virtual void draw() = 0;

	/*
	@brief	処理開始に必要なパラーメータの初期設定や処理を行う関数

	@param[in]	なし

	@return		なし

	@note	記載なし。クラス構造を作るための型として作成したので、宣言のみ
	@note	シーン内の各オブジェクトの初期化などの処理をこの関数へまとめる
	@note	このクラスを実際に使用する事がないので仮想化している
　　*/
	virtual  void start() = 0;


private:

};
#endif // !__BASICSCENE_H__
