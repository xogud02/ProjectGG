#include "GridObject.h"
#include "SpriteFactory.h"	 

USING_NS_CC;

const TileMap& GridObject::getTiles() const{
	return tiles;
}

void GridObject::addTile(cocos2d::Sprite * tile, GridPosition position) {
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
