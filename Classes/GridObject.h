#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include <unordered_map>

using TileMap = std::unordered_map<GridPosition, cocos2d::Sprite*>;

class GridObject :public cocos2d::Node{
	TileMap tiles;
public:
	CREATE_FUNC(GridObject);
	const TileMap& getTiles() const;
	void addTile(cocos2d::Sprite* tile, GridPosition);
};
