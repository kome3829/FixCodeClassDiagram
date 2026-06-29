#ifndef __ITEMOBJECTMANAGER_H__
#define __ITEMOBJECTMANAGER_H__

#include "DxLib.h"
#include "define.h"
#include "Deta.h"
#include "math.h"
#include "Object.h"
#include "Player.h"
// --- アイテムドロップ位置補正 ---
#define DROP_OFFSET_X (20)
// EXPアイテム拡散
#define DROP_EXP_ITEM_AMOUNT (6)
#define DROP_EXP_ANGLE_STEP (60)
#define DROP_EXP_START_INDEX (1)
// 乱数
#define DROP_RATE_RANDOM_MAX (100)
class ItemObjectManager
{
  public:
	ItemObjectManager();
	~ItemObjectManager();
	void action();
	void draw();
	void start();

	// アイテムオブジェクトの設置
	void setItemObject(int setPositionX, int setPositionY, int setAngle,
	                   int itemObjectType,
	                   Player *player_p); 
	// オブジェクトクラスの当たり判定実行関数
	void hitCheck(Player *player_p); 

	Object *mItemObjects[MAX_BULLET_NUMBER]; // アイテムオブジェクト

	// アイテムドロップ関数
	void dropItem(int itemX, int itemY, int itemType,
	              Player *player_p); 

  private:
};
#endif // !__ITEMOBJECTMANAGER_H__
