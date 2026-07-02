#include "ItemObjectManager.h"
/*
@brief	コンストラクタ

@param	なし
@return		なし

@note     各オブジェクトクラスのインスタンスを生成している
*/
ItemObjectManager::ItemObjectManager()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i] = new Object();
	}
}
/*
@brief	デストラクタ

@param	なし
@return		なし

@note
インスタンス生成した 全てのオブジェクトクラスのdeleteを行い、メモリの開放を行う
*/
ItemObjectManager::~ItemObjectManager()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		delete mItemObjects[i];
		mItemObjects[i] = nullptr;
	}
}
/*
@brief	メインループで実行する更新処理を行う関数

@param     	なし
@return     なし
@note		生成した全てのオブジェクトクラスの更新処理を行う
*/
void ItemObjectManager::action()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i]->action();
	}
}
/*
@brief	描画ループで実行する描画処理を行う関数

@param	なし
@return		なし

@note	生成した全てのオブジェクトクラスの描画処理を行う

*/
void ItemObjectManager::draw()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i]->draw();
	}
}
/*
@brief	処理開始に必要なパラーメータの初期設定や処理を行う関数

@param	なし
@return		なし
@note	生成した全てのオブジェクトクラスの初期設定や処理を行う

*/
void ItemObjectManager::start()
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		mItemObjects[i]->start();
	}
}
/*
@brief	アイテムドロップに必要な位置やフラグなどのパラメータ設定を行う関数

@param[in]	int setPositionX			:出現させるX座標
@param[in]	int setPositionY			:出現させるY座標
@param[in]	int setAngle				:発射角度
@param[in]	int itemObjectType			:アイテムの種類
@param[in]	Player *player_p		:プレイヤークラスのポインタ

@return		なし
@note

- フラグ（mIsActive）が true の場合、設定処理を行わず次のオブジェクトクラスへ
-
フラグ（mIsActive）をtrueにしてaction関数及びオブジェクトクラスの関数の処理を有効にしている
- 引数で渡された位置や種類などをもとに各パラメータを設定する
@warning
-　プレイヤーの位置変数は常に更新されるため、ポインタで直接参照している。
*/
void ItemObjectManager::setItemObject(int setPositionX, int setPositionY,
                                      int setAngle, int itemObjectType,
                                      Player *player_p)
{
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		if (mItemObjects[i]->mIsActive) // すでに使用されているクラスをスルー
		{
			continue;
		}

		mItemObjects[i]->mX = setPositionX;
		mItemObjects[i]->mY = setPositionY;
		mItemObjects[i]->mPlayerX = &(player_p->mX);
		mItemObjects[i]->mPlayerY = &(player_p->mY);
		mItemObjects[i]->mMovementAngle = setAngle;
		mItemObjects[i]->mItemObjectType = itemObjectType;
		// 経験値アイテムの場合、速度を設定
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
		mItemObjects[i]->mIsActive = true; // 有効判定をtrue

		return; // 設置処理が行われたのでループを抜ける
	}
}

/*
@brief	アイテムオブジェクトとプレイヤーの当たり判定を行う関数

@param[in]	Player *player_p :プレイヤークラスのポインタ
@return		なし
@note

- プレイヤークラスのフラグ(mIsActive)がtrueの場合のみ処理を実行する
- プレイヤーに一定距離まで近づいたら当たったと判定する

*/
void ItemObjectManager::checkHit(Player *player_p)
{
	if (!player_p->mIsActive) // 有効化されていない場合は処理を行わない
	{
		return;
	}
	for (int i = 0; i < MAX_BULLET_NUMBER; i++)
	{
		if (mItemObjects[i]->checkHit(
		        player_p)) // オブジェクトクラスの判定処理を呼び出し
		{
			return; // 当たり判定があった場合、プレイヤークラスで効果反映処理を行うためにループを抜ける
		}
	}
}

/*
@brief	アイテムのドロップを管理する関数

@param[in]	int itemX			:出現させるX座標
@param[in]	int itemY			:出現させるY座標
@param[in]	int itemType		:出現させるアイテムの種類
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
           - 30パーセントの確率でドロップするようにしている。

      -
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
		// 30％の確率で実行
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
