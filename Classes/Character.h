#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include "Status.h"
#include <vector>
#include <functional>
#include "SpriteTileEnums.h"
#include "GridObject.h"

class GridLayer;
class GaugeBar;

enum class MoveType{
	Stop,
	Hold,
	AttackOnNotice,
	Move
};

class Character : public cocos2d::Sprite {
protected:
	static std::unordered_map<GridObject*, std::function<void(Character*)>> onMove;
	int team = 0;

	Status status;
	std::shared_ptr<GaugeBar> hpBar;
	cocos2d::Vec2 nextPos;
	std::queue<GridPosition> path;
	MoveType currentMoveType = MoveType::Stop;
	CharacterDirection currentDirection = CharacterDirection::DOWN;
	GridPosition currentGridPosition;

	Character* target = nullptr;
	void releaseTarget();

	virtual void onAttackBegin();

	CharacterDirection getNextDirection(GridPosition nextPosition);
	virtual void onMoveBegin(GridPosition nextPosition, CharacterDirection nextDirection);
	void movePath(float);
	bool init() override;
public:
	static void addMoveListener(GridObject*, std::function<void(Character*)>);

	void tryToMove(GridPosition);
	bool tryToJump(GridPosition);
	void stopMove();

	const int SCALE;
	GridPosition getCurrentGridPosition() const;

	cocos2d::Vec2 getCenturalPosition();

	static Character* create(int scale = 1);

	Character(int scale);

	virtual void removeFromParentAndCleanup(bool) override;
	ChracterCondition getCondition() const;
	CharacterDirection getCurrentDirection();
	bool isEnemy(Character*);

	virtual void setTarget(Character* target);
	Character* getTarget();
	void setMoveType(MoveType moveType);
	virtual bool isInAttackRange(Character* who) const;
	virtual void setPosition(const cocos2d::Vec2& v) override;

	virtual void hit(Character* by, int damage);
	virtual ~Character();

private:
	int buffCount = 0;
public:
	virtual void buff(int power, float time, cocos2d::Node* icon);
};
