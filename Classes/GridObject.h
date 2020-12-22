#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include <unordered_map>
#include "TileType.h"

using TileMap = std::unordered_map<GridPosition, cocos2d::Sprite*>;
using TileTypeMap = std::unordered_map<GridPosition, TileType>;

class GridObject :public cocos2d::Node{
	TileMap tiles;
	TileTypeMap tileTypes;
public:
	CREATE_FUNC(GridObject);
	const TileMap& getTiles() const;
	const TileTypeMap& getTileTypes() const;
	void addTile(GridPosition, TileType type = TileType::Block, cocos2d::Sprite* tile = nullptr);
};
