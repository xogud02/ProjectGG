#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Character.h"
#include <vector>

using GridPosition = std::pair<int, int>;

class Grid :public cocos2d::LayerColor{
	std::vector<std::vector<bool>> movableGrid;
	const float UNIT_SIZE;
	Character* player;

	bool isMovable(int row, int col);

	void setPlayer(Character* character);

	GridPosition vecToGrid(cocos2d::Vec2 position);
	cocos2d::Vec2 gridToPosition(GridPosition gridPosition);
public:
	int getRows();
	int getCols();
	static Grid* createGrid(int rows, int cols);
	Grid(int rows, int cols, float unitSize);
	void showGrid();
	bool onTouch(cocos2d::Touch* t, cocos2d::Event* e);
};
