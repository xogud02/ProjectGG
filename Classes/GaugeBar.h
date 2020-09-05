#pragma once

#include "cocos2d.h"

class GaugeBar : public cocos2d::DrawNode {
	int maxValue = 100;
	int value = maxValue;
	cocos2d::DrawNode* fill;
	cocos2d::DrawNode* barcode;
public:
	virtual bool init() override;
	static GaugeBar* create(cocos2d::Size size, int value = 100);
	void setValue(int value);
	void setMaxValue(int value);
	void updateBar();
};
