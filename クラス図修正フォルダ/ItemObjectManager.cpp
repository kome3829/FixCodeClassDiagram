#include "ItemObjectManager.h"

ItemObjectManager::ItemObjectManager()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i] = new Object();
	}
}

ItemObjectManager::~ItemObjectManager()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		delete mItemObjects[i];
		mItemObjects[i] = nullptr;
	}
}

void ItemObjectManager::action()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i]->action();
	}
}

void ItemObjectManager::draw()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i]->draw();
	}
}

void ItemObjectManager::start()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i]->start();
	}
}
/*
@brief	アイテムドロップに必要な位置やフラグなどのパラメータ設定を行う関数

@param[in]	int setPositionX			:出現させるのX座標
@param[in]	int setPositionY			:出現させるのY座標
@param[in]	int setAngle				:発射角度
@param[in]	int itemObjectType			:アイテムの種類
@param[in]	double* playerPositionX		:プレイヤー位置のｘ座標ポインタ
@param[in]	double* playerPositionY		:プレイヤー位置のｙ座標ポインタ

@return		実行状態:bool:実行した　true/実行してない false

@note

- フラグ（flg）が true の場合は再設定を行わず false を返す
- フラグ（flg）をtrueにしてaction関数及びdraw関数の処理を有効にしている
- 引数で渡された位置や種類などをもとに各パラメータを設定する
@warning
- ドロップ中か判定できるように、返り値は bool 型としている
-　プレイヤーの位置変数は常に更新されるため、ポインタで直接参照している。
*/
void ItemObjectManager::setItemObject(int setPositionX, int setPositionY,
                                      int setAngle, int itemObjectType,
                                      Player *player_p)
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		if (mItemObjects[i]->mIsActive)//すでに使用されているクラスをスルー		
		{
			continue;
		}

		mItemObjects[i]->mX = setPositionX;
		mItemObjects[i]->mY = setPositionY;
		mItemObjects[i]->mPlayerX = &(player_p->mX);
		mItemObjects[i]->mPlayerY = &(player_p->mY);
		mItemObjects[i]->mMovementAngle = setAngle;
		mItemObjects[i]->mItemObjectType = itemObjectType;
		// 経験値アイテムの場合、速度をを設定
		if (itemObjectType == OBJECT_EXP)
		{
			// ラジアンに変換
			double movementRadian =
			    mItemObjects[i]->mMovementAngle / DEGREE_TO_RADIAN_DIVISOR * PI;
			mItemObjects[i]->mVectorX =
			    cos(movementRadian) * OBJECT_EXP_FAST_SPEED;
			mItemObjects[i]->mVectorY =
			    sin(movementRadian) * OBJECT_EXP_FAST_SPEED;
		}
		mItemObjects[i]->mIsActive = true;//有効判定をtrue

		return;
	}
}

/*
@brief	各アイテムオブジェクトがプレイヤーへ当たったか判定を行う関数

@param[in]	Object* itemObject :オブジェクトクラスのインスタンスポインタ
@param[in]	int*    score		:ゲームスコアのポインタ

@return		当たったアイテムの種類:int
@note  経験値アイテム　OBJECT_EXP  1
@note  回復アイテム　OBJECT_LIFE 2
@note  無敵アイテム　OBJECT_STAR 3
@note

- 当たったアイテムオブジェクトを示すため、返り値をint型としている
- アイテムオブジェクトのフラグ(flg)がtrueの場合のみ処理を実行する
- プレイヤーに一定距離まで近づいたら当たったと判定する
- 当たった場合、アイテムオブジェクトに応じて処理を分岐する

    1. 経験値アイテム
        - ショットパワーを増やす

    2. 回復アイテム
        - 残機を1つ増やす
        - 残機が最大の場合は増やさない

    3. 無敵アイテム
        - 無敵にする
        - 無敵の場合は残り時間を最大値まで戻す

*/
void ItemObjectManager::hitCheck(Player *player_p)
{
	if (!player_p->mIsActive)
	{
		return;
	}
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		if (!mItemObjects[i]->mIsActive )
		{
			continue;
		}
		// アイテムと当たり判定をとるクラスとの距離を計算
		int distX = abs((int)(player_p->mX - mItemObjects[i]->mX));
		int distY = abs((int)(player_p->mY - mItemObjects[i]->mY));

		// 一定距離まで近づいたら当たったと判定
		if (distX < ITEM_OBJECT_WIDTH && distY < ITEM_OBJECT_HEIGHT)
		{
			if (!player_p->mIsItemHit) // ヒット判定中ではなければ
			{
				player_p->mIsItemHit = true; // ヒット判定を有効
				player_p->mHitItemType = mItemObjects[i]->mItemObjectType; // ヒットしたアイテム種類を伝達


			}
			// アイテムの無効化及びリセット
			mItemObjects[i]->reset();

			return;
		}
	}
}

/*
@brief	アイテムのドロップを管理する関数

@param[in]	int x			:出現させるのX座標
@param[in]	int y			:出現させるのY座標
@param[in]	int itemtype	:出現させるアイテムの種類
@param[in]	 Player *player_p	:プレイヤークラスのポインタ


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
void ItemObjectManager::dropItem(int itemX, int itemY, int itemType,
                                 Player *player_p)
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

			setItemObject(itemX, itemY, expItemAngle, OBJECT_EXP, player_p);
		}

		break;

	case OBJECT_LIFE: // 回復アイテム

		// 乱数の取得
		randomNumber = GetRand(DROP_RATE_RANDOM_MAX);

		// 10％の確率で実行
		if (randomNumber <= OBJECT_LIFE_DROP_PAESENT)
		{
			setItemObject(itemX, itemY, 0, OBJECT_LIFE, player_p);
		}

		break;

	case OBJECT_STAR: // 無敵アイテム

		// 乱数の取得
		randomNumber = GetRand(DROP_RATE_RANDOM_MAX);
		// 30％野確率で実行
		if (randomNumber <= OBJECT_STAR_DROP_PAESENT)
		{
			setItemObject(itemX + DROP_OFFSET_X, itemY, 0, OBJECT_STAR,
			              player_p);
		}

		break;

	default:
		break;
	}
}
