#include "Skill.h"
#include "Character.h"
#include "WeaponSpriteFactory.h"

USING_NS_CC;

Skill::Skill(float coolDown):
	coolDown(coolDown)
{}

void Skill::setOwner(Character * owner) {
	this->owner = owner;
}

bool Skill::onTarget(Character * to) {
	CCLOG("unemplemented onTarget Called");
	return false;
}

bool Skill::nonTarget(const cocos2d::Vec2 & to) {
	CCLOG("unemplemented nonTarget Called");
	return false;
}

cocos2d::Node * Skill::getIcon() {
	return Sprite::createWithSpriteFrame(WeaponSpriteFactory::sword());
}

float Skill::getCooldown() {
	return coolDown;
}

