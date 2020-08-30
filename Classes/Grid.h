#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GridPosition.h"
#include <vector>

using UINT = unsigned int;

enum class TileType : std::uint8_t {
	Floor = 0,
	Water,
	Block
};

class Character;
class Player;
class Grid : public cocos2d::LayerColor {
public:
	const float UNIT_SIZE;

	bool isMovable(const int row, const int col, const int size = 1) const;
	bool isMovable(const GridPosition gridPosition, const int size = 1) const;
	GridPosition vecToGrid(const cocos2d::Vec2 position) const;
	cocos2d::Vec2 gridToPosition(const GridPosition gridPosition) const;
	int getRows() const;
	int getCols() const;
	Grid(const int rows, const int cols, const float unitSize);
	void showGrid();
	bool onTouch(const cocos2d::Touch* t, const cocos2d::Event* e);
	void occupyArea(const GridPosition, const int size = 1, const bool occupy = true);
	bool isOccupied(const GridPosition, const int size = 1);
	void focusTo(cocos2d::Vec2 position);

	void addChild(cocos2d::Sprite* node);
	void addChild(cocos2d::Sprite* node, int zOrder);

	static Grid* create(const int rows, const int cols);

	void touched(Character* who);
	void setPlayer(Player* character);

private:
	std::vector<std::vector<bool>> occupiedGrid;
	std::unordered_map<GridPosition, TileType> tiles;

	const UINT row;
	const UINT coloum;
	Player* player;

	bool isValidPosition(const GridPosition position) const;
	bool isValidPosition(const int row, const int col) const;
};
