#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include <array>
#include "GridPosition.h"

using UINT = unsigned int;

enum class TileType : std::uint8_t {
	Floor = 0,
	Water,
	Block
};

class Character;
class Grid : public cocos2d::LayerColor {
public:
	const float UNIT_SIZE;

	bool isMovable(int row, int col,int size = 1);
	bool isMovable(GridPosition gridPosition, int size = 1);
	GridPosition vecToGrid(cocos2d::Vec2 position);
	cocos2d::Vec2 gridToPosition(GridPosition gridPosition);
	int getRows();
	int getCols();
	Grid(int rows, int cols, float unitSize);
	void showGrid();
	bool onTouch(cocos2d::Touch* t, cocos2d::Event* e);
	cocos2d::Vec2 getPositionOffset();
	void occupyArea(const GridPosition, const int size = 1, bool occupy = true);

	static Grid* createGrid(int rows, int cols);

private:
	std::vector<std::vector<bool>> occupiedGrid;
	std::unordered_map<GridPosition, TileType> tiles;

	const UINT row;
	const UINT coloum;
	Character* player;

	cocos2d::DrawNode* debugGrid;

	void setPlayer(Character* character);
};
