#pragma once

#include <functional>
#include "cocos2d.h"
class Character;

class Skill {
protected:
	float coolDown = 3.0f;
	Character* owner;

public:
	Skill() = default;
	Skill(float coolDown);
	void setOwner(Character* owner);
	virtual void onTarget(Character* to);
	virtual void nonTarget(const cocos2d::Vec2& to);
	float getCooldown();
	virtual cocos2d::Node* getIcon();
};
