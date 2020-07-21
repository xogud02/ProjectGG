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

	void focusTo(cocos2d::Vec2 position);

	void addChild(cocos2d::Node* node) override;
	void addChild(cocos2d::Node* node, int zOrder) override;

	static Grid* create(const int rows, const int cols);

private:
	std::vector<std::vector<bool>> occupiedGrid;
	std::unordered_map<GridPosition, TileType> tiles;

	const UINT row;
	const UINT coloum;
	Character* player;

	cocos2d::DrawNode* debugGrid;

	bool isValidPosition(const GridPosition position) const;
	bool isValidPosition(const int row, const int col) const;
	void setPlayer(Character* character);
};
