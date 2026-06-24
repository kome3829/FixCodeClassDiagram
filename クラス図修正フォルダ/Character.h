#ifndef __CHARACTER_H__
#define __CHARACTER_H__

class Character
{
  public:
	Character() {};
	~Character() {};
	double mX = 0;              // x座標
	double mY = 0;              // y座標
	int mWidth = 0;             // 判定をとる横サイズ
	int mHeight = 0;            // 判定をとる縦サイズ
	bool mIsTakeDamage = false; // ダメージ判定
	bool mIsActive = false;

  private:
};

#endif // !__CHARACTER_H__
