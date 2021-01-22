#include "GridObject.h"
#include "SpriteFactory.h"	 

USING_NS_CC;

void GridObject::testTrigger() {
	CCAssert(false, "not implemented");
}

const TileMap& GridObject::getTiles() const{
	return tiles;
}

const TileTypeMap & GridObject::getTileTypes() const {
	return tileTypes;
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
	CCAssert(false, "not implemented");
}
