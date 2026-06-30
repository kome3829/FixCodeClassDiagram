#ifndef __TITLESCENE_H__
#define __TITLESCENE_H__

#include "BasicScene.h"
class TitleScene : public BasicScene
{
public:
    TitleScene();
    ~TitleScene();
     int action() ;//イコールゼロをつけると全て仮想化になるので一度つけるだけでいい
     void draw() ;
     void start();
     bool mIsOperationGuideScreen;//操作説明画面切り替え判定


private:

};

#endif // !__TITLESCENE_H__
