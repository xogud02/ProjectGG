#include "FireBall.h"
#include "MonsterSpriteFactory.h"
#include "Character.h"
#include "GridLayer.h"
#include "Grid.h"

USING_NS_CC;

Sprite* createFireBall() {
	auto ret = Sprite::create();
	ret->runAction(MonsterSpriteFactory::createMonsterAnimation(ElementalType::FireBall));
	return ret;
}

Node * FireBall::getIcon() {
	return createFireBall();
}

bool FireBall::nonTarget(const Vec2 & to) {
	auto from = owner->getPosition();
	auto direction = (to - from).getNormalized();
	auto layer = GridLayer::getInstance();
	auto fireBall = createFireBall();
	layer->addChild(fireBall, 1);
	fireBall->setPosition(owner->getPosition());
	
	auto degree = CC_RADIANS_TO_DEGREES(Vec2::angle(Vec2(0, 1), direction));
	if (direction.x < 0) {
		degree = -degree;
	}
	fireBall->setFlippedY(true);
	fireBall->setRotation(degree);

	auto grid = Grid::getInstance();
	auto key = "fireBall";
	layer->schedule([fireBall, delta = direction * layer->UNIT_SIZE * 0.5f, layer, grid, key](float) mutable {
		auto currentPosition = fireBall->getPosition();
		if (!grid->isValidPosition(layer->vecToGrid(currentPosition))) {
			layer->unschedule(key);
			CCLOG("fireBall done");
			return;
		}
		auto newPos = fireBall->getPosition() + delta;
		fireBall->setPosition(newPos);
	}, key);
	return true;
}
