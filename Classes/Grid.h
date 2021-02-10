#pragma once

#include "GridPosition.h"
#include <unordered_map>
#include <unordered_set>
#include "TileType.h"
#include "cocos2d.h"

class GridObject;
class Character;

class Grid {
	std::unordered_map<GridPosition, TileType> tileTypes;
	std::unordered_map<GridPosition, std::unordered_set<GridObject*>> triggerObjects;
	std::unordered_map<GridPosition, cocos2d::Node*> occupied;//TODO maybe needs root object class
	static Grid* instance;

public:
	static Grid* getInstance();

	const int rows, cols;

	Grid(int rows, int cols);
	bool isTrigger(const GridPosition& gridPosition) const;
	void trigger(const GridPosition& gridPosition);
	bool isMovableTile(const GridPosition& gridPosition, const int size = 1) const;
	bool isValidPosition(const GridPosition& position, const int size = 1) const;

	void occupyArea(Character* by, const GridPosition&);
	void unOccupyArea(Character* by, const GridPosition&);
	bool isOccupiable(const GridPosition&, const int size = 1)const;

	Character* getOccupiedCharacter(const GridPosition&) const;
};
