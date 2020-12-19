#include "FireBall.h"
#include "MonsterSpriteFactory.h"
#include "Character.h"
#include "GridLayer.h"
#include "Grid.h"
#include <unordered_set>

USING_NS_CC;
using namespace std;

Sprite* createFireBall() {
	auto ret = Sprite::create();
	ret->runAction(MonsterSpriteFactory::createMonsterAnimation(ElementalType::FireBall));
	return ret;
}

Node * FireBall::getIcon() {
	return createFireBall();
}

bool FireBall::nonTarget(const Vec2 & to) {
	auto from = owner->getCenturalPosition();
	auto direction = (to - from).getNormalized();
	auto layer = GridLayer::getInstance();
	auto fireBall = createFireBall();
	layer->addChild(fireBall, 1);
	fireBall->setPosition(from);
	
	auto degree = CC_RADIANS_TO_DEGREES(Vec2::angle(Vec2(0, 1), direction));
	if (direction.x < 0) {
		degree = -degree;
	}
	fireBall->setFlippedY(true);
	fireBall->setRotation(degree);

	auto grid = Grid::getInstance();
	auto key = "fireBall";
	layer->schedule([fireBall, delta = direction * layer->UNIT_SIZE * 0.5f, layer, grid, key, owner = owner, hitted = unordered_set<Character*>()](float) mutable {
		auto currentPosition = fireBall->getPosition();
		auto currentGridPosition = layer->vecToGrid(currentPosition);
		if (!grid->isValidPosition(currentGridPosition)) {
			layer->unschedule(key);
			fireBall->removeFromParent();
			return;
		}
		auto test = grid->getOccupiedCharacter(currentGridPosition);
		if (test && test != owner && hitted.insert(test).second) {
			test->hit(owner, 1);
		}
		auto newPos = fireBall->getPosition() + delta;
		fireBall->setPosition(newPos);
	}, key);
	return true;
}
