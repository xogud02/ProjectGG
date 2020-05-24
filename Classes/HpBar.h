#pragma once

#include "cocos2d.h"

class HPBar : public cocos2d::Sprite {
	cocos2d::Sprite* fill;
public:
	static HPBar* createWithColor(cocos2d::Color3B color);
	void setHP(float ratio);
};
