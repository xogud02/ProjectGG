#pragma once

#include "cocos2d.h"

class GridUnit;
class Weapon : public cocos2d::Sprite {
	GridUnit* target = nullptr;
public:
	static Weapon* create();
	virtual bool init() override;
	void setTarget(GridUnit* c);
	void swing(float duration);
};
