#include "GridObject.h"
#include "SpriteFactory.h"	 
#include "Character.h"

USING_NS_CC;
using namespace std;

bool GridObject::isInTrigger(Character * who) const {
	auto size = who->SCALE;
	auto pos = who->getCurrentGridPosition();
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			auto test = GridPosition(pos.row + dr, pos.col + dc) - currentGridPosition;
			auto itr = tileTypes.find(test);
			if (itr != tileTypes.cend() && itr->second == TileType::EventTrigger) {
				return true;
			}
		}
	}
	return false;
}

GridObject::GridObject() {
	Character::addMoveListener(this, [this](auto c) {testTrigger(c); });
}

bool GridObject::testTrigger(Character* who) const{
	bool isTriggering = isInTrigger(who);
	bool wasTriggering = triggering.find(who) != triggering.cend();
	if (isTriggering == wasTriggering) {
		return false;
	}

	if (isTriggering) {
		onTriggerIn(who);
	} else {
		onTriggerOut(who);
	}

	return true;
}

void GridObject::addTile(GridPosition position, TileType tileType, Sprite* tile) {
	tileTypes[position] = tileType;
	if (!tile) {
		return;
	}

	addChild(tile);
	auto size = SpriteFactory::unitSize.width;
	tile->setPosition(size * position.col, size * position.row);
	
	auto before = tiles[position];
	if (before) {
		before->removeFromParentAndCleanup(true);
	}
	tile->setAnchorPoint(Vec2::ZERO);
	tiles[position] = tile;
}

void GridObject::setGridPosition(GridPosition newGridPosition) {
	currentGridPosition = newGridPosition;
}

void GridObject::addChild(cocos2d::Node * child) {
	Node::addChild(child);
	auto object = dynamic_cast<GridObject*>(child);
	if (!object) {
		return;
	}
	auto size = SpriteFactory::unitSize.width;
	auto cPos = object->currentGridPosition + currentGridPosition;
	object->setPosition(size * cPos.col, size * cPos.row);
}
