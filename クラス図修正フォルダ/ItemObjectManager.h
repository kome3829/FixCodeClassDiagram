#pragma once
#include "DxLib.h"
#include "define.h"
#include "deta.h"
#include "math.h"
#include "object.h"
#include "player.h"

class ItemObjectManager {
public:
  ItemObjectManager();
  ~ItemObjectManager();
  void action();
  void draw();
  void start();

  bool setItemObject(int setPositionX, int setPositionY, int setAngle,
                     int itemObjectType, Player* player_p ); // アイテムオブジェクトの設置
  void hitCheck(Player* player_p);      // オブジェクトクラスの当たり判定実行関数

  Object* mItemObjects[MAX_BULLET_NUMBER]; // アイテムオブジェクト

  void dropItem(int itemX, int itemY, int itemType, Player* player_p);//ドロップ関数



private:
};