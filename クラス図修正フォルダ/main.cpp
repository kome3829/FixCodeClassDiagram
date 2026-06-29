#include "DxLib.h"
#include"myLib/MyLib.h"
#include"myLib/key.h"
#include "math.h"
#include "define.h"
#include "Deta.h"
#include "BasicScene.h"
#include "TitleScene.h"
#include"GameMainScene.h"
#include"result.h"

#define ONE_FRAME_MILLISECOND (16)//１フレームをミリ秒に換算した値
int sceneType; //現在のシーンの種類
int previousSceneType;//action関数更新前のシーン
BasicScene* sceneArray[SCENE_MAX];//sceneのインスタンス生成ポインタ
Data* Data::mInstance = nullptr;//主な場所に一つだけ
/*
@brief	ゲーム開始時の初期化処理を行う関数

@param	なし
@return		なし

- 各シーンクラスのインスタンスを生成する
- 最初に実行するシーンのstart関数を実行する
- データクラスのインスタンス生成を含む load関数 を実行する

*/
void initialize()
{
	sceneArray[SCENE_TITLE] = new TitleScene();//タイトルシーンポインタ。クラスインスタンス生成
	sceneArray[SCENE_GAMEMAIN] = new GameMainScene();//ゲームメインシーンポインタ。クラスインスタンス生成
	sceneArray[SCENE_RESULT] = new ResultScene();//リザルトシーンポインタ。クラスインスタンス生成
	sceneType = SCENE_TITLE;//初期シーンのセット
	//　データの読み込み
	Data::getInstance()->load();
	sceneArray[sceneType]->start();
}
/*
@brief	終了時のメモリ解放処理を行う関数

@param	なし
@return		なし

- 生成した各シーンクラスのインスタンスを delete する必要がある
- データクラスのインスタンスも解放する必要がある
- ※ 現状では解放処理が不足している

*/
void release()
{
}

/*
@brief	メインループ内の更新処理を行う関数

@param	なし
@return		なし

- 入力データの更新処理を行う
- 現在のシーンの action関数 を実行する
- action関数 の戻り値によってシーン遷移を判定する
- シーンが変更された場合、新しいシーンの start関数 を実行する

*/
void action()
{
	updateKeyBuff();//キー入力更新
	updateControllerKeyBuff();//コントローラの入力更新
	//switch文を使わずに一文で書ける
	 previousSceneType = sceneType;//アクション更新前のシーン番号を記憶
	sceneType = sceneArray[sceneType]->action();//シーンのアクション関数更新
	if (previousSceneType != sceneType)
	{//シーン番号が変わったら自動的にstartが呼ばれる
		sceneArray[sceneType]->start();
	}
}

/*
@brief	メインループ内の描画処理を行う関数

@param	なし
@return		なし

- 現在のシーンの draw関数を実行する

*/
void draw()
{
	//switch文を使わずに一文で書ける
	sceneArray[sceneType]->draw();//シーンのドロー関数更新	
}

/*
@brief	プログラムのメイン関数
	
- WinMainはWindowsアプリの開始関数で、起動情報を受け取る引数を持つ

@return		プログラムの終了コード:int
@note return 0 → 正常終了  return -1 → エラー終了

@note 

- 画面設定や DxLib の初期化を行う
- DxLibのテンプレとして使っている
- ダブルバッファ設定を行う
- initialize関数 を実行する

- メインループ処理
	- action関数 → draw関数 の順で毎フレーム実行する
	- escspeキーが押されたメインループを終了している。

- 終了処理
	- release関数を実行し、リソースを解放する

*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 画面モードの設定
	SetGraphMode(MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT, 32);
	//SetGraphMode(1280, 720, 32);//1Kサイズ 画面比16:9
	// タイトルを test に変更
	SetMainWindowText("ウインドウタイトル");
	// ウインドウモードに変更
	//ChangeWindowMode(false);
	ChangeWindowMode(true);
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	//ゲームループ開始前に画面の設定
	// 描画先画面を裏にする(ダブルバッファ
	SetDrawScreen(DX_SCREEN_BACK);
	initialize();//ゲームの初期化
	//フレームレート
	while (ProcessMessage() == 0)//エラーが帰って来ない限り繰り返せ
	{
		int startTime = GetNowCount();
		action();
		ClearDrawScreen();//画面のクリア1
		draw();
		ScreenFlip(); // 裏画面の内容を表画面に反映します
		//ecspeボタンが押されたらこの無限ループを終了（break）する
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
		//フレームを安定させる処理
		int nowTime = GetNowCount();
		int waitTime = ONE_FRAME_MILLISECOND - (nowTime - startTime);//経過時間から1フレームまでの残り時間を計算。
		if (waitTime > 0)
		{
			WaitTimer(waitTime);//1フレームよりも早く処理が終った場合、1フレーム分まで待つ
		}
	}
	release();//開放
	DxLib_End();				// ＤＸライブラリ使用の終了処理
	return 0;				// ソフトの終了 
}