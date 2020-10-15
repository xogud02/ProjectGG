#include "Grid.h"
#include "GridObject.h"

using namespace std;

Grid* Grid::instance = nullptr;

Grid * Grid::getInstance() {
	return instance;
}

Grid::Grid(int rows, int cols) : rows(rows), cols(cols), occupiedGrid(rows, vector<bool>(cols, false)) {
	instance = this;
}

bool Grid::isMovable(const GridPosition& gridPosition, const int size) const {
	int row = gridPosition.row, col = gridPosition.col;
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			auto current = GridPosition(row + dr, col + dc);
			if (!isValidPosition(current)) {
				return false;
			}

			const auto tileItr = tiles.find(current);
			if (tileItr != tiles.cend() && tileItr->second != TileType::Floor) {
				return false;
			}
		}
	}
	return true;
}

bool Grid::isValidPosition(const GridPosition& position) const {
	int row = position.row, col = position.col;
	return 0 <= row && row < rows && 0 <= col && col < cols;
}

void Grid::addObject(GridObject * gridObject, GridPosition position) {
	for (auto p : gridObject->getTiles()) {
		tiles[position + p.first] = TileType::Block;
	}
}

void Grid::occupyArea(const GridPosition& position, const int size, const bool occupy) {
	const int row = position.row;
	const int col = position.col;

	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			int r = row + dr, c = col + dc;
			if (!isValidPosition(GridPosition(r,c))) {
				return;
			}
			occupiedGrid[r][c] = occupy;
		}
	}
}

bool Grid::isOccupied(const GridPosition& position, const int size) const{
	int r = position.row;
	int c = position.col;
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			if (occupiedGrid[r + dr][c + dc]) {
				return true;
			}
		}
	}
	return false;
}
