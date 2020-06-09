#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Character.h"
#include <vector>

using GridPosition = std::pair<int, int>;

enum class TileTypes : std::uint8_t {
	Default = 0,
	Water,
	Max
};

struct Tile {
public:
	Tile(
		const uint16_t xval,
		const uint16_t yval,
		bool isblocked = false,
		TileTypes type = TileTypes::Default) :
		X(xval),
		Y(yval),
		m_IsBlocked(isblocked),
		m_Type(type)
	{}

	bool IsBlocked()		{ return m_IsBlocked; }
	TileTypes GetType()		{ return m_Type; }

	const uint16_t X;
	const uint16_t Y;

private:
	bool m_IsBlocked = false;
	TileTypes m_Type;
};

class Grid :public cocos2d::LayerColor{
	std::vector<std::vector<bool>> movableGrid;
	Character* player;

	void setPlayer(Character* character);

public:
	const float UNIT_SIZE;
	bool isMovable(int row, int col);
	GridPosition vecToGrid(cocos2d::Vec2 position);
	cocos2d::Vec2 gridToPosition(GridPosition gridPosition);
	int getRows();
	int getCols();
	static Grid* createGrid(int rows, int cols);
	Grid(int rows, int cols, float unitSize);
	void showGrid();
	bool onTouch(cocos2d::Touch* t, cocos2d::Event* e);
};
