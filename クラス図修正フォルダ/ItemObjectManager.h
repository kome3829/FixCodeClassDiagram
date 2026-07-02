#ifndef __ITEMOBJECTMANAGER_H__
#define __ITEMOBJECTMANAGER_H__

#include "Deta.h"
#include "Object.h"
#include "Player.h"
#include "define.h"
// --- アイテムドロップ位置補正 ---
constexpr int DROP_OFFSET_X = 20;
// EXPアイテム拡散
constexpr int DROP_EXP_ITEM_AMOUNT = 6;
constexpr int DROP_EXP_ANGLE_STEP = 60;
constexpr int DROP_EXP_START_INDEX = 1;

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
	                   int itemObjectType, Player *player_p);
	// オブジェクトクラスの当たり判定実行関数
	void checkHit(Player *player_p);

	// アイテムドロップ関数
	void dropItem(int itemX, int itemY, int itemType, Player *player_p);

  private:
	Object *mItemObjects[MAX_BULLET_NUMBER]; // アイテムオブジェクト
};
#endif // !__ITEMOBJECTMANAGER_H__
