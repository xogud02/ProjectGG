#pragma once

#include "GridPosition.h"
#include <unordered_map>
#include <unordered_set>
#include "TileType.h"

class GridObject;
class Character;

class Grid {
	std::unordered_map<GridPosition, TileType> tileTypes;
	std::unordered_map<GridPosition, std::unordered_set<GridObject*>> triggerObjects;
	std::unordered_set<GridObject*> gridObjects;
	std::unordered_map<GridPosition, Character*> occupiedCharacter;
	static Grid* instance;

public:
	static Grid* getInstance();

	const int rows, cols;

	Grid(int rows, int cols);
	bool isTrigger(const GridPosition& gridPosition) const;
	void trigger(const GridPosition& gridPosition);
	bool isMovableTile(const GridPosition& gridPosition, const int size = 1) const;
	bool isValidPosition(const GridPosition& position, const int size = 1) const;
	void addObject(GridObject* gridObject, GridPosition position);

	void occupyArea(Character* by, const GridPosition&);
	void unOccupyArea(Character* by, const GridPosition&);
	bool isOccupiable(const GridPosition&, const int size = 1)const;

	Character* getOccupiedCharacter(const GridPosition&) const;
};
