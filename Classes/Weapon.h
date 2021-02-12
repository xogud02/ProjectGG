#pragma once

#include "cocos2d.h"

class Unit;
class Weapon : public cocos2d::Sprite {
	Unit* target = nullptr;
public:
	static Weapon* create();
	virtual bool init() override;
	void setTarget(Unit* c);
	void swing(float duration);
};
