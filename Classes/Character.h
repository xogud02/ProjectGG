#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include <vector>
#include <functional>

class Grid;
class HPBar;

class Character : public cocos2d::Sprite {
protected:
	int speed = 5;
	int maxHP = 100;
	int hp = 100;
	HPBar* hpBar;
	cocos2d::Vec2 nextPos;
	std::queue<GridPosition> path;

	GridPosition currentGridPosition;

	Grid* grid = nullptr;
	Grid* getGrid();

	virtual void moveSingleGrid(float);
	void movePath(float);
	bool init() override;
public:
	virtual void moveTo(cocos2d::Vec2);
	void tryToMove(GridPosition);

	const int SCALE;

	static Character* create(int scale = 1);

	Character(int scale);

	virtual void hit(int damage);
	virtual ~Character();
};
