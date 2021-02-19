#include "GridUnits.h"
#include "SpriteFactory.h"
#include "GridLayer.h"

USING_NS_CC;

void GridUnits::addUnit(GridUnit *unit) {
	units.insert(unit);
}

void GridUnits::addTile(GridPosition position, TileType tileType, cocos2d::Sprite * tile) {
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

UnitSet GridUnits::getUnits() const {
	return units;
}

void GridUnits::setGridPosition(GridPosition newGridPosition) {
	currentGridPosition = newGridPosition;
	auto size = GridLayer::getInstance()->UNIT_SIZE;
	Node::setPosition(size * currentGridPosition.col, size * currentGridPosition.row);
}
