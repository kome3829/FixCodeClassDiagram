#ifndef __CHARACTER_H__
#define __CHARACTER_H__

class Character
{
  public:
	Character() {};
	~Character() {};
	double mX;   // x座標
	double mY;   // y座標
	int mWidth;  // 判定をとる横サイズ
	int mHeight; // 判定をとる縦サイズ
	bool mIsTakeDamage;//ダメージ判定
	bool mIsActive;
  private:
};

#endif // !__CHARACTER_H__
