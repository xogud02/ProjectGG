#include "Monster.h"
#include "SpriteFactory.h"
#include "HpBar.h"

USING_NS_CC;

Monster::Monster(float scale) : Character(scale)
{
}

Monster * Monster::create(float unitSize, float scale)
{
	Monster* ret = new Monster(scale);
	if (!ret) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}


	ret->setAnchorPoint(Vec2::ZERO);

	ret->autorelease();
	float characterSize = unitSize * ret->SCALE;

	ret->runAction(SpriteFactory::slime());
	ret->setScale(SpriteFactory::getUnitScale(characterSize));

	Size size = ret->getContentSize();
	float width = size.width;

	auto draw = DrawNode::create();
	draw->drawRect(Vec2::ZERO, Vec2(size.width, size.height), Color4F::RED);
	ret->addChild(draw);

	return ret;
}


