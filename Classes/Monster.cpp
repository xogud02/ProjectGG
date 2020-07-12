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
	runAction(SpriteFactory::slime());
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


