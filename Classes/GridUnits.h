#pragma once

#include "cocos2d.h"
#include <unordered_set>
#include <unordered_map>
#include "TileType.h"
#include "GridPosition.h"

class GridUnit;
using UnitSet = std::unordered_set<GridUnit*>;
using TileMap = std::unordered_map<GridPosition, cocos2d::Sprite*>;
using TileTypeMap = std::unordered_map<GridPosition, TileType>;

class GridUnits : public cocos2d::Node{
	UnitSet units;
	TileMap tiles;
	TileTypeMap tileTypes;
	GridPosition currentGridPosition;
public:
	CREATE_FUNC(GridUnits);
	void addUnit(GridUnit*);
	void addTile(GridPosition, TileType type = TileType::Block, cocos2d::Sprite* tile = nullptr);
	UnitSet getUnits() const;
	void setGridPosition(GridPosition newGridPosition);
};