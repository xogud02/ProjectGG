#include "Grid.h"
#include "GridObject.h"
#include "Character.h"
#include <algorithm>

using namespace std;

Grid* Grid::instance = nullptr;

Grid * Grid::getInstance() {
	return instance;
}

Grid::Grid(int rows, int cols) : rows(rows), cols(cols){
	instance = this;
}

bool Grid::isTrigger(const GridPosition & gridPosition) const {
	auto itr = tileTypes.find(gridPosition);
	return itr != tileTypes.cend() && itr->second == TileType::EventTrigger;
}

bool Grid::testTrigger(const Character* who) const {
	return any_of(gridObjects.begin(), gridObjects.end(), [who](auto gridObject) {return gridObject->testTrigger(who); });
}

void gridLoop(const GridPosition& gridPosition, const int size, function<bool(const GridPosition&)> keepLoop) {
	for (int dr = 0; dr < size; ++dr) {
		for (int dc = 0; dc < size; ++dc) {
			if (!keepLoop(GridPosition(gridPosition.row + dr, gridPosition.col + dc))) {
				return;
			}
		}
	}
}

void Grid::trigger(const GridPosition & gridPosition) {
	auto test = triggerObjects.find(gridPosition);
	if (test == triggerObjects.cend()) {
		return;
	}
	CCASSERT(false, "not implemented");
	//test->second->onTriggerIn();
}

bool Grid::isMovableTile(const GridPosition& gridPosition, const int size) const {
	if (!isValidPosition(gridPosition, size)) {
		return false;
	}

	bool ret = true;
	gridLoop(gridPosition, size, [&ret, this](const GridPosition& current) mutable{
		auto tileItr = tileTypes.find(current);
		if (tileItr == tileTypes.cend()) {
			return true;
		}

		auto currentTile = tileItr->second;
		if (currentTile != TileType::EventTrigger && currentTile != TileType::Floor) {
			ret = false;
			return false;
		}
		return true;
	});
	return ret;
}

bool Grid::isValidPosition(const GridPosition& position, int size) const {
	int row = position.row, col = position.col;
	return 0 <= row && row + size -1 < rows && 0 <= col && col + size -1 < cols;
}

void Grid::addObject(GridObject * gridObject, GridPosition position) {
	gridObjects.insert(gridObject);
	gridObject->setGridPosition(position);
	for (auto p : gridObject->getTileTypes()) {
		auto currentPosition = position + p.first;
		auto currentTile = p.second;
		tileTypes[currentPosition] = currentTile;
	}
}

void Grid::occupyArea(Character * by, const GridPosition& position) {
	auto size = by->SCALE;
	if (!isOccupiable(position, size)) {
		return;
	}
	by->retain();
	gridLoop(position, size, [this, by](const auto& pos) mutable {
		occupiedCharacter[pos] = by;
		return true;
	});
}

void Grid::unOccupyArea(Character * by, const GridPosition& position) {
	auto size = by->SCALE;
	auto itr = occupiedCharacter.find(position);
	if (itr == occupiedCharacter.cend() || itr->second != by) {
		return;
	}
	by->release();
	gridLoop(position, size, [this, by](const auto& pos) mutable {
		occupiedCharacter.erase(pos);
		return true;
	});
}

bool Grid::isOccupiable(const GridPosition& position, const int size) const {
	if (!isValidPosition(position, size)) {
		return false;
	}

	bool ret = true;
	gridLoop(position, size, [this, &ret](const auto& pos) mutable {
		if (occupiedCharacter.find(pos) != occupiedCharacter.cend()) {
			ret = false;
			return false;
		}
		return true;
	});

	return ret;
}

Character* Grid::getOccupiedCharacter(const GridPosition &position) const {
	auto itr = occupiedCharacter.find(position);
	if (itr == occupiedCharacter.cend()) {
		return nullptr;
	}

	return itr->second;
}
