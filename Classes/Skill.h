#pragma once

#include <functional>
#include "cocos2d.h"
class Unit;

class Skill {
protected:
	float coolDown = 3.0f;
	Unit* owner;

public:
	Skill() = default;
	Skill(float coolDown);
	void setOwner(Unit* owner);
	virtual bool onTarget(Unit* to);
	virtual bool nonTarget(const cocos2d::Vec2& to);
	float getCooldown();
	virtual cocos2d::Node* getIcon();
};
