#ifndef __JFUTILS_H__
#define __JFUTILS_H__

#include "cocos2d.h"

typedef std::vector<cocos2d::Vec2> PosArray;
extern PosArray fillRectWithPoints(cocos2d::Rect rect, double spacing);
extern cocos2d::Texture2D* getDefaultTexture();

#endif