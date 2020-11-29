#include "Blink.h"
#include "GridLayer.h"
#include "Character.h"
#include "MonsterSpriteFactory.h"

USING_NS_CC;

Node* ::Blink::getIcon() {
	auto blink = Sprite::create();
	blink->runAction(MonsterSpriteFactory::createMonsterAnimation(ElementalType::LightBlink));
	return blink;
}

void ::Blink::nonTarget(const Vec2& to) {
	owner->tryToJump(GridLayer::getInstance()->vecToGrid(to));
}
