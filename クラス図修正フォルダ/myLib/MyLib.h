#pragma once
#include "DxLib.h"
#define DEFAULT_SET_FONT_SIZE	(16)
#define HALF(x)			((x)/2)

void drawCenterString(int drawCentarPositionX, int drawCentarPositionY, const char* string, int color);
void setFontSize(int fontSize);