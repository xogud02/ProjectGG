#include "GridObject.h"
#include "SpriteFactory.h"	 

USING_NS_CC;

const TileMap& GridObject::getTiles() const{
	return tiles;
}

const TileTypeMap & GridObject::getTileTypes() const {
	return tileTypes;
}

void GridObject::addTile(GridPosition position, TileType tileType, Sprite* tile) {
	addChild(tile);
	auto size = SpriteFactory::unitSize.width;
	tile->setPosition(size * position.col, size * position.row);
	
	auto before = tiles[position];
	if (before) {
		before->removeFromParentAndCleanup(true);
	}
	tile->setAnchorPoint(Vec2::ZERO);
	tiles[position] = tile;
	tileTypes[position] = tileType;
}
