#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include <unordered_map>
#include "TileType.h"
#include <unordered_set>

using TileMap = std::unordered_map<GridPosition, cocos2d::Sprite*>;
using TileTypeMap = std::unordered_map<GridPosition, TileType>;
class Character;

class GridObject :public cocos2d::Node{
	TileMap tiles;
	TileTypeMap tileTypes;
	std::unordered_set<Character*> triggering;
	GridPosition currentGridPosition;
	bool isInTrigger(Character* who) const;
public:
	GridObject();

	std::function<void(Character*)> onTriggerIn;
	std::function<void(Character*)> onTriggerOut;
	bool testTrigger(Character* who) const;

	CREATE_FUNC(GridObject);
	void addTile(GridPosition, TileType type = TileType::Block, cocos2d::Sprite* tile = nullptr);
	void setGridPosition(GridPosition newGridPosition);
	GridPosition getGridPosition() const;
	virtual void addChild(cocos2d::Node* child) override;
};
