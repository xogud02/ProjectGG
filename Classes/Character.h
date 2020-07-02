#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include <vector>
#include <functional>

class Grid;

class Character : public cocos2d::Sprite {
	int speed = 5;
	cocos2d::Vec2 nextPos;
	cocos2d::Action* currentAction;
	std::queue<GridPosition> path;

	std::vector<std::pair<std::function<bool(float)>, cocos2d::Action*>> directions;
	GridPosition currentGridPosition;

	Grid* grid = nullptr;
	Grid* getGrid();

	void move(float);
	void movePath(float);
public:
	void moveTo(cocos2d::Vec2);
	void tryToMove(GridPosition);

	const int SCALE;

	static Character* create(float unitSize, int scale = 2);

	Character(int scale);
	virtual ~Character();
};
