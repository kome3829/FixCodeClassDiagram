#pragma once
#include "basicScene.h"
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
