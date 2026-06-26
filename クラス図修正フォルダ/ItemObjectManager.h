#ifndef __ITEMOBJECTMANAGER_H__
#define __ITEMOBJECTMANAGER_H__

#include "DxLib.h"
#include "define.h"
#include "deta.h"
#include "math.h"
#include "object.h"
#include "player.h"
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

	void setItemObject(int setPositionX, int setPositionY, int setAngle,
	                   int itemObjectType,
	                   Player *player_p); // アイテムオブジェクトの設置
	void hitCheck(Player *player_p); // オブジェクトクラスの当たり判定実行関数

	Object *mItemObjects[MAX_BULLET_NUMBER]; // アイテムオブジェクト

	/// <summary>
	/// アイテムドロップ関数
	/// </summary>
	/// <param name="x=ドロップのX座標"></param>
	/// <param name="y=ドロップのY座標"></param>
	/// <param name="itemtype=アイテムの種類"></param>
	///
	void dropItem(int itemX, int itemY, int itemType,
	              Player *player_p); // ドロップ関数

  private:
};
#endif // !__ITEMOBJECTMANAGER_H__
