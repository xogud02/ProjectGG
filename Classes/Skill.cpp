#include "Skill.h"
#include "Character.h"

Skill::Skill(float coolDown):
	coolDown(coolDown)
{}

void Skill::setOwner(Character * owner) {
	this->owner = owner;
}

void Skill::onTarget(Character * to) {
}

void Skill::nonTarget(const cocos2d::Vec2 & to) {
}

