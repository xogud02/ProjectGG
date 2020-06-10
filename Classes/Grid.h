#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include <array>

using GridPosition = std::pair<int, int>;//TODO ¡Æ©ø¨ù¡¾
using UINT = unsigned int;

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

class Character;
class Grid : public cocos2d::LayerColor {
public:
	const float UNIT_SIZE;

	bool isMovable(int row, int col);
	GridPosition vecToGrid(cocos2d::Vec2 position);
	cocos2d::Vec2 gridToPosition(GridPosition gridPosition);
	int getRows();
	int getCols();
	Grid(int rows, int cols, float unitSize);
	void showGrid();
	bool onTouch(cocos2d::Touch* t, cocos2d::Event* e);
	cocos2d::Vec2 getPositionOffset();
	void occupyArea(const GridPosition, const int size = 1);

	static Grid* createGrid(int rows, int cols);

private:
	std::vector<std::vector<bool>> movableGrid;

	const UINT row;
	const UINT coloum;
	Character* player;

	cocos2d::DrawNode* debugGrid;

	void setPlayer(Character* character);
};
