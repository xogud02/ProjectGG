#include "Monster.h"
#include "SpriteFactory.h"
#include "HpBar.h"

USING_NS_CC;

Monster::Monster(int scale) : Character(scale)
{
}

bool Monster::init()
{
	if (!Character::init()) {
		return false;
	}
	setName("monster");
	runAction(SpriteFactory::slime());
	schedule(CC_SCHEDULE_SELECTOR(Monster::moveRandom), 1.0f);
	return true;
}

Monster * Monster::create(float scale)
{
	Monster* ret = new Monster(scale);
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	return ret;
}

void Monster::moveRandom(float dt){
	tryToMove(currentGridPosition + GridPosition(cocos2d::random() % 3 - 1, cocos2d::random() % 3 - 1));
}



