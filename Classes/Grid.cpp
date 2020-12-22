#include "Grid.h"
#include "GridObject.h"
#include "Character.h"

using namespace std;

Grid* Grid::instance = nullptr;

Grid * Grid::getInstance() {
	return instance;
}

Grid::Grid(int rows, int cols) : rows(rows), cols(cols){
	instance = this;
}

bool Grid::isMovableTile(const GridPosition& gridPosition, const int size) const {
	int row = gridPosition.row, col = gridPosition.col;
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			auto current = GridPosition(row + dr, col + dc);
			if (!isValidPosition(current)) {
				return false;
			}

			const auto tileItr = tileTypes.find(current);
			if (tileItr != tileTypes.cend() && tileItr->second != TileType::Floor) {
				return false;
			}
		}
	}
	return true;
}

bool Grid::isValidPosition(const GridPosition& position, int size) const {
	int row = position.row, col = position.col;
	return 0 <= row && row + size -1 < rows && 0 <= col && col + size -1 < cols;
}

void Grid::addObject(GridObject * gridObject, GridPosition position) {
	for (auto p : gridObject->getTileTypes()) {
		tileTypes[position + p.first] = p.second;
	}
}

void Grid::occupyArea(Character * by, const GridPosition& position) {
	auto size = by->SCALE;
	if (!isOccupiable(position, size)) {
		return;
	}
	by->retain();
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			auto next = GridPosition(position.row + dr, position.col + dc);
			occupiedCharacter[next] = by;
		}
	}
}

void Grid::unOccupyArea(Character * by, const GridPosition& position) {
	auto size = by->SCALE;
	auto itr = occupiedCharacter.find(position);
	if (itr == occupiedCharacter.cend() || itr->second != by) {
		return;
	}
	by->release();
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			auto next = GridPosition(position.row + dr, position.col + dc);
			occupiedCharacter.erase(next);
		}
	}
}

bool Grid::isOccupiable(const GridPosition& position, const int size) const {
	if (!isValidPosition(position, size)) {
		return false;
	}

	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			auto next = GridPosition(position.row + dr, position.col + dc);
			if (occupiedCharacter.find(next) != occupiedCharacter.cend()) {
				return false;
			}
		}
	}

	return true;
}

Character * Grid::getOccupiedCharacter(const GridPosition &position) const {
	auto itr = occupiedCharacter.find(position);
	if (itr == occupiedCharacter.cend()) {
		return nullptr;
	}

	return itr->second;
}
