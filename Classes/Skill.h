#pragma once

#include <functional>
#include "cocos2d.h"
class GridUnit;

class Skill {
protected:
	float coolDown = 3.0f;
	GridUnit* owner;

public:
	Skill() = default;
	Skill(float coolDown);
	void setOwner(GridUnit* owner);
	virtual bool onTarget(GridUnit* to);
	virtual bool nonTarget(const cocos2d::Vec2& to);
	float getCooldown();
	virtual cocos2d::Node* getIcon();
};
