#pragma once

#include "cocos2d.h"

class GaugeBar{
	int maxValue = 100;
	int value = maxValue;
public:
	GaugeBar(int value = 100);
	void setValue(int value);
	void setMaxValue(int value);
	void drawBar(cocos2d::DrawNode* canvas, cocos2d::Vec2 position, cocos2d::Size size);
};
