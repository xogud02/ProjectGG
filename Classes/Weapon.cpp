#include "Weapon.h"
#include "SpriteFactory.h"

USING_NS_CC;

Weapon * Weapon::create()
{
	auto ret = new Weapon();
	if (!ret || !ret->initWithSpriteFrame(SpriteFactory::sword())) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	ret->setName("Weapon");
	ret->setAnchorPoint(Vec2(1, 0));
	return ret;
}

void Weapon::swing(){
	runAction(RotateBy::create(0.5f, Vec3(0, 0, 360)));
}
