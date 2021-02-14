#include "ThrowWeapon.h"
#include "Player.h"
#include "WeaponSpriteFactory.h"
#include "Weapon.h"
#include "GridLayer.h"

USING_NS_CC;

ThrowWeapon::ThrowWeapon(float coolDown):Skill(coolDown) {
}

bool ThrowWeapon::onTarget(GridUnit * to) {
	auto player = dynamic_cast<Player*>(owner);
	auto weapon = WeaponSpriteFactory::sword();

	if (player) {
		weapon = player->getWeapon()->getSpriteFrame();
	}
	auto throwed = Sprite::createWithSpriteFrame(weapon);
	GridLayer::getInstance()->addChild(throwed,1);
	auto key = "throwing";
	throwed->setPosition(owner->getPosition());
	throwed->schedule([owner = owner, throwed,key,to, time = 0.5f, remain = 0.5f, start = owner->getPosition()](float dt)mutable{
		if (remain <= 0) {
			to->hit(owner, 1);
			throwed->unschedule(key);
			throwed->removeFromParent();
			return;
		}
		remain -= dt;
		auto dest = to->getPosition();
		auto delta = dest - throwed->getPosition();
		throwed->setRotation(-CC_RADIANS_TO_DEGREES(delta.getAngle()) + 135);//TODO move angle logic
		throwed->setPosition(start.lerp(dest, 1 - remain / time));
	},key);

	return true;
}
