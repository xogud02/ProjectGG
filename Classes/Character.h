#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include "Status.h"
#include <vector>
#include <functional>
#include "SpriteTileEnums.h"

class Grid;
class GaugeBar;

enum class MoveType{
	Stop,
	Hold,
	AttackOnNotice,
	Move
};

class Character : public cocos2d::Sprite {
protected:
	int team = 0;

	Status status;
	GaugeBar* hpBar;
	cocos2d::Vec2 nextPos;
	std::queue<GridPosition> path;
	MoveType currentMoveType = MoveType::Stop;

	CharacterDirection currentDirection = CharacterDirection::DOWN;

	GridPosition currentGridPosition;

	Character* target = nullptr;
	void releaseTarget();

	Grid* grid = nullptr;
	Grid* getGrid();

	virtual void onAttackBegin();

	CharacterDirection getNextDirection(GridPosition nextPosition);
	virtual void onMoveBegin(GridPosition nextPosition, CharacterDirection nextDirection);
	void movePath(float);
	bool init() override;
public:
	void tryToMove(GridPosition);

	const int SCALE;

	static Character* create(int scale = 1);

	Character(int scale);

	virtual void removeFromParentAndCleanup(bool) override;
	ChracterCondition getCondition();
	bool isEnemy(Character*);

	void setTarget(Character* target);
	void setMoveType(MoveType moveType);
	virtual bool isInAttackRange(Character* who) const;
	virtual void setPosition(const cocos2d::Vec2& v) override;

	virtual void hit(Character* by, int damage);
	virtual ~Character();
};
