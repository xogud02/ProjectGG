#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include <vector>
#include <functional>

class Grid;
class HPBar;

enum class AttackResult {
	None,
	Normal,
	Die,
	Block,
	Dodgy,
	Immune
};
class Character : public cocos2d::Sprite {
protected:
	int speed = 5;
	float attackInterval = 1;
	int maxHP = 100;
	int hp = 100;
	HPBar* hpBar;
	cocos2d::Vec2 nextPos;
	std::queue<GridPosition> path;

	GridPosition currentGridPosition;

	Grid* grid = nullptr;
	Grid* getGrid();

	virtual void onMoveBegin(GridPosition nextPosition);
	void movePath(float);
	bool init() override;
public:
	void tryToMove(GridPosition);

	const int SCALE;

	static Character* create(int scale = 1);

	Character(int scale);

	void setTarget(Character* target);
	virtual bool isInAttackRange(Character* who) const;
	virtual AttackResult attack(Character* c);
	virtual void setPosition(const cocos2d::Vec2& v) override;

	virtual void hit(int damage);
	virtual ~Character();
};
