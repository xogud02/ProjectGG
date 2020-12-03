#include "FireBall.h"
#include "MonsterSpriteFactory.h"

USING_NS_CC;

Node * FireBall::getIcon() {
	auto ret = Sprite::create();
	ret->runAction(MonsterSpriteFactory::createMonsterAnimation(ElementalType::FireBall));
	return ret;
}

bool FireBall::nonTarget(const Vec2 & to) {

	return true;
}
