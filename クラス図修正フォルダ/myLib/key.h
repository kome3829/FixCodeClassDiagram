#pragma once
#include "DxLib.h"

#define INPUT_BUFF_SIZE	(256)

void updateKeyBuff();					//キーボード入力のバフ更新
bool checkPushkey(int keycode);			//キーボードの通常押し判定
bool checkSinglePushKey(int keycode);	//キーボードの単押し判定
bool checkReleaseKey(int keycode);		//キーボードの離し判定

void updateControllerKeyBuff();					//コントローラ入力のバフ更新
bool checkPushControllerKey(int keycode);		//コントローラの通常押し判定
bool checkSinglePushControllerKey(int keycode);	//コントローラの単押し判定
bool checkReleaseControllerKey(int keycode);	//コントローラの離し判定

