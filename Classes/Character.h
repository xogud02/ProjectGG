#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include "Status.h"
#include <vector>
#include <functional>

class Grid;
class GaugeBar;

enum class AttackResult {
	None,
	Normal,
	Die,
	Block,
	Dodgy,
	Immune
};

enum class MoveType{
	Stop,
	Hold,
	AttackOnNotice,
	Move
};

class Character : public cocos2d::Sprite {
protected:
	int speed = 5;
	float noticeRange = 5;
	float attackInterval = 1;
	Status status;
	GaugeBar* hpBar;
	cocos2d::Vec2 nextPos;
	std::queue<GridPosition> path;
	MoveType currentMoveType = MoveType::Stop;

	GridPosition currentGridPosition;

	Character* target = nullptr;
	void releaseTarget();

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
	void setMoveType(MoveType moveType);
	virtual bool isInAttackRange(Character* who) const;
	virtual AttackResult attack(Character* c);
	virtual void setPosition(const cocos2d::Vec2& v) override;

	virtual void hit(Character* by, int damage);
	virtual ~Character();
};
