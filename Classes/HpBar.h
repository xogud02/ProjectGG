#pragma once

#include "cocos2d.h"

class HPBar : public cocos2d::DrawNode {
	cocos2d::DrawNode* fill;
public:
	static HPBar* create(cocos2d::Size size);
	void setHP(float ratio);
};
