#pragma once

#include <functional>
class Character;

using OnSkill = std::function<void(Character*)>;
class Skill {
	float coolDown;
	OnSkill skillAction;
public:
	Skill(float coolDown, OnSkill onSkill);
	void useSkill(Character* by);
};
