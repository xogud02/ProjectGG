#include "Character.h"
#include "HpBar.h"
#include "FightScene.h"

USING_NS_CC;

Character* Character::createCharacter(float unitSize, int scale) {
	Character* ret = new Character(scale);
	if (!ret || !ret->initWithFile("Characters/Warrior.png",Rect(0,0,16,16))) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	ret->autorelease();
	float characterSize = unitSize * ret->SCALE;
	ret->setContentSize(Size(characterSize, characterSize));
	HPBar* hpBar = HPBar::createWithColor(Color3B::GREEN);

	Size size = ret->getContentSize();
	float width = size.width;
	hpBar->setContentSize(Size(width, width / 6));
	hpBar->setHP(1);
	hpBar->setPosition(Vec2(width / 2,0));
	ret->addChild(hpBar);
	return ret;
}

Character::Character(int scale):SCALE(scale)
{
}

void Character::tick(float dt) {
	auto currentPos = getPosition();
	auto direction = (nextPos - currentPos);
	auto normalized = direction.getNormalized();

	float moveDist = speed * dt;

	if (direction.length() < moveDist) {
		unschedule(CC_SCHEDULE_SELECTOR(Character::tick));
		CCLOG("arrive at (%f, %f)", currentPos.x, currentPos.y);
		return;
	}
	setPosition(currentPos + normalized * moveDist);
}

void Character::move(Vec2 position)
{
	nextPos = position;
	if (this->isScheduled(CC_SCHEDULE_SELECTOR(Character::tick))) {
		CCLOG("moving!!");
	}
	this->schedule(CC_SCHEDULE_SELECTOR(Character::tick));
}
