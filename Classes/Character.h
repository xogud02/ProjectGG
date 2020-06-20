#pragma once

#include "cocos2d.h"
#include <vector>
#include <functional>


using GridPosition = std::pair<int, int>;
class Grid;

class Character : public cocos2d::Sprite {
	int speed = 100;
	cocos2d::Vec2 nextPos;
	cocos2d::Action* currentAction;
	std::queue<GridPosition> path;

	std::vector<std::pair<std::function<bool(float)>, cocos2d::Action*>> directions;
	GridPosition currentGridPosition;

	Grid* getGrid();

	void move(float);
	void movePath(float);
public:
	void moveTo(cocos2d::Vec2);
	void tryToMove(GridPosition);

	const int SCALE;

	static Character* create(float unitSize, int scale = 2);

	static cocos2d::Action* createMoveAction(std::string file, int r);

	Character(int scale);
	virtual ~Character();
};
