
#include "key.h"
 

char gKeyBuffArray[INPUT_BUFF_SIZE];//キーボード入力バフ配列
char gPreviousFrameKeyBuffArray[INPUT_BUFF_SIZE];//前フレームキーボード入力バフ配列
char gControllerkeyBuff;//コントロール入力バフ
char gPreviousFrameControllerkeyBuff;///前フレームコントロール入力バフ

/*
	@brief	キーバッファの更新を行う

	@param[in]	なし
	@return		なし

	@note	ゲームの更新処理で必ず実行する
	@note	キーボードから入力されたデータを前フレームと現フレームで保存
	@note	この関数を実行しないとファイル内のキー入力判定関数が使用できなくなる
	@note	1：押されている　０：押されていないとしてデータが入っている
*/
void updateKeyBuff()
{
	//一フレーム前のデータを保存
	for (int i = 0; i < INPUT_BUFF_SIZE; i++)
	{
		gPreviousFrameKeyBuffArray[i]=gKeyBuffArray[i];
	}
	GetHitKeyStateAll(gKeyBuffArray);
}

/*
	@brief	 キーが押され続けているかを判定する

	@param[in]	int keycode 入力キーコード
	@return	   状態:bool

	@note	押されている true / 押されていない false
	@note	現フレームでキーが押されているか判定している
	@note	引数でどのキーを判定するか設定している
*/
bool checkPushkey(int keycode)
{
	// keycodeに対応したバッファの状態を返す
	if (gKeyBuffArray[keycode] == 1)
	{
		return true;
	}
	return false;
}

/*
	@brief	 キーが1回押されたかを判定する関数

	@param[in]	int keycode 入力キーコード
	@return	   状態:bool

	@note	単押された true / 単押しされてない false
	@note	キーが前フレームが押されてなく現フレームが押されているか判定している
	@note	押された瞬間だけ判定している
	@note	引数でどのキーを判定するか設定している
*/
bool checkSinglePushKey(int keycode)
{
	//前のフレームが押されてなく今のフレームが押されている
	if (gPreviousFrameKeyBuffArray[keycode] == 0 && gKeyBuffArray[keycode] == 1)
	{
		return true;
	}
	return false;
}
/*
	@brief	 キーが離されたかを判定する関数

	@param[in]	int keycode 入力キーコード
	@return	   状態:bool

	@note	離された true / 離されてない false
	@note	キーが前のフレーム押されていて今のフレーム離したか判定
	@note	離された瞬間だけ判定している
	@note	引数でどのキーを判定するか設定している
*/
bool checkReleaseKey(int keycode)
{
	//前のフレーム押されていて今のフレーム離された
	if (gPreviousFrameKeyBuffArray[keycode] == 1 && gKeyBuffArray[keycode] == 0)
	{
		return true;
	}
	return false;
}


/*
	@brief	コントローラーキーバッファの更新を行う

	@param[in]	なし
	@return		なし

	@note	ゲームの更新処理で必ず実行する
	@note	コントローラーから入力されたデータを前フレームと現フレームで保存
	@note	この関数を実行しないとファイル内のコントローラー入力判定関数が使用できなくなる
*/
void updateControllerKeyBuff()
{
	//一フレーム前のデータを保存
		gPreviousFrameControllerkeyBuff = gControllerkeyBuff;
		gControllerkeyBuff = GetJoypadInputState(DX_INPUT_PAD1);

}

/*
	@brief	 ボタンが押され続けているかを判定する

	@param[in]	int controllerkeycode 入力ボタンコード
	@return	   状態:bool

	@note   押されている true / 押されていない false
	@note	現フレームでキーが押されているか判定している
	@note	引数でどのボタンを判定するか設定している
*/
bool checkPushControllerKey(int controllerkeycode)
{
	// AND演算をして結果が０でなければそのボタンが押されている事を示す
	if ((gControllerkeyBuff & controllerkeycode) != 0)
	{
		return true;
	}
	return false;
}
/*
	@brief	 ボタンが1回押されたかを判定する関数

	@param[in]	int controllerkeycode 入力ボタンコード
	@return	   状態:bool

	@note   単押された true / 単押しされてない false 
	@note	ボタンが前フレームが押されてなく現フレームが押されているか判定している
	@note	押された瞬間だけ判定している
	@note	引数でどのボタンを判定するか設定している
*/


bool checkSinglePushControllerKey(int controllerkeycode)
{
	// AND演算をして結果が０でなければそのボタンが押されている事を示す
	//前のフレームが押されてなく今のフレームが押されているとtrue
	if ((gPreviousFrameControllerkeyBuff & controllerkeycode) == 0 && (gControllerkeyBuff & controllerkeycode) != 0)
	{
		return true;
	}
	return false;
}

/*
	@brief	 ボタンが離されたかを判定する関数

	@param[in]	int controllerkeycode 入力ボタンコード
	@return	   状態:bool

	@note   離された true / 離されてない false 
	@note	ボタンが前のフレーム押されていて今のフレーム離したか判定
	@note	離された瞬間だけ判定している
	@note	引数でどのボタンを判定するか設定している
*/

bool checkReleaseControllerKey(int controllerkeycode)
{
	// AND演算をして結果が０でなければそのボタンが押されている事を示す
	//前のフレーム押されていて今のフレーム離された
	if ((gPreviousFrameControllerkeyBuff & controllerkeycode) != 0 && (gControllerkeyBuff & controllerkeycode) == 0)
	{
		return true;
	}
	return false;
}
