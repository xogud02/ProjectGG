#pragma once

#include "GridPosition.h"
#include <unordered_map>

enum class TileType : std::uint8_t {
	Floor = 0,
	Water,
	Block
};

class GridObject;

class Grid {
	std::unordered_map<GridPosition, TileType> tiles;
	std::vector<std::vector<bool>> occupiedGrid;
	static Grid* instance;

public:
	static Grid* getInstance();

	const int rows, cols;

	Grid(int rows, int cols);
	bool isMovable(const GridPosition& gridPosition, const int size = 1) const;
	bool isValidPosition(const GridPosition& position) const;
	void addObject(GridObject* gridObject, GridPosition position);

	void occupyArea(const GridPosition&, const int size = 1, const bool occupy = true);
	bool isOccupied(const GridPosition&, const int size = 1) const;
};
