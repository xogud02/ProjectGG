#include "Skill.h"
#include "Character.h"

Skill::Skill(float coolDown, OnSkill onSkill):
	coolDown(coolDown),
	skillAction(onSkill)
{}

void Skill::onTarget(Character * to) {
}

void Skill::nonTarget(const cocos2d::Vec2 & to) {
}

