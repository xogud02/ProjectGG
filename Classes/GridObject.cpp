#include "GridObject.h"
#include "SpriteFactory.h"	 
#include "Character.h"
#include "GridLayer.h"

USING_NS_CC;
using namespace std;

bool GridObject::isInTrigger(Character * who) const {
	auto size = who->SCALE;
	auto pos = who->getCurrentGridPosition();
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			auto mine = getGridPosition();
			auto test = GridPosition(pos.row + dr, pos.col + dc) - mine;
			auto itr = tileTypes.find(test);
			if (itr != tileTypes.cend() && itr->second == TileType::EventTrigger) {
				return true;
			}
		}
	}
	return false;
}

GridObject::GridObject() {
	Character::addMoveListener(this);
}

GridObject::~GridObject() {
	Character::removeMoveListener(this);
}

void GridObject::testTrigger(Character* who) {
	bool isTriggering = isInTrigger(who);
	bool wasTriggering = triggering.find(who) != triggering.cend();
	if (isTriggering == wasTriggering) {
		return;
	}

	if (isTriggering) {
		onTriggerIn(who);
	} else {
		onTriggerOut(who);
	}

	return;
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
	auto size = GridLayer::getInstance()->UNIT_SIZE;
	setPosition(size * currentGridPosition.col, size * currentGridPosition.row);
}

GridPosition GridObject::getGridPosition() const {
	auto parent = dynamic_cast<const GridObject*>(getParent());
	if (parent) {
		return currentGridPosition + parent->getGridPosition();
	}
	return currentGridPosition;
}

void GridObject::addChild(Node * child) {
	Node::addChild(child);
	auto object = dynamic_cast<GridObject*>(child);
	if (!object) {
		return;
	}
	auto size = SpriteFactory::unitSize.width;
	auto cPos = object->currentGridPosition;
	object->setPosition(size * cPos.col, size * cPos.row);
}
