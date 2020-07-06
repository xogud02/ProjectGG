#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include <vector>
#include <functional>

class Grid;

class Character : public cocos2d::Sprite {
	int speed = 5;
	cocos2d::Vec2 nextPos;
	std::queue<GridPosition> path;

	GridPosition currentGridPosition;

	Grid* grid = nullptr;
	Grid* getGrid();

	void move(float);
	void movePath(float);
public:
	virtual void moveTo(cocos2d::Vec2);
	void tryToMove(GridPosition);

	const int SCALE;

	static Character* create(float unitSize, int scale = 2);

	Character(int scale);
	virtual ~Character();
};
