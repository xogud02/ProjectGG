#pragma once

#include <functional>
#include "cocos2d.h"
class Character;

using OnSkill = std::function<void(Character*)>;
class Skill {
	float coolDown;
	OnSkill skillAction;
	Character* owner;
public:
	Skill(float coolDown, OnSkill onSkill);
	void onTarget(Character* to);
	void nonTarget(const cocos2d::Vec2& to);
};
