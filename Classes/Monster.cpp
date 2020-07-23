#include "Monster.h"
#include "SpriteFactory.h"
#include "HpBar.h"

USING_NS_CC;

Monster::Monster(float scale) : Character(scale)
{
}

bool Monster::initCharacter(float unitSize)
{
	if (!Character::initCharacter(unitSize)) {
		return false;
	}
	setName("monster");
	runAction(SpriteFactory::slime());
	schedule(CC_SCHEDULE_SELECTOR(Monster::moveRandom), 1.0f);
	return true;
}

Monster * Monster::create(float unitSize, float scale)
{
	Monster* ret = new Monster(scale);
	if (!ret || !ret->initCharacter(unitSize)) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}

	return ret;
}

void Monster::moveRandom(float dt){
	tryToMove(currentGridPosition + GridPosition(random() % 3 - 1, random() % 3 - 1));
}



