#pragma once

#include "cocos2d.h"
#include "GridPosition.h"
#include "Status.h"
#include <vector>
#include <functional>
#include "SpriteTileEnums.h"
#include "TileType.h"
#include <unordered_map>
#include <unordered_set>

class GridLayer;
class GaugeBar;

enum class MoveType{
	Stop,
	Hold,
	AttackOnNotice,
	Move
};

using TileMap = std::unordered_map<GridPosition, cocos2d::Sprite*>;
using TileTypeMap = std::unordered_map<GridPosition, TileType>;

class Unit : public cocos2d::Sprite {
protected:
	static std::unordered_set<Unit*> onMove;
	int team = 0;

	Status status;
	std::shared_ptr<GaugeBar> hpBar;
	cocos2d::Vec2 nextPos;
	std::queue<GridPosition> path;
	MoveType currentMoveType = MoveType::Stop;
	CharacterDirection currentDirection = CharacterDirection::DOWN;
	GridPosition currentGridPosition;

	Unit* target = nullptr;
	void releaseTarget();

	virtual void onAttackBegin();

	CharacterDirection getNextDirection(GridPosition nextPosition);
	virtual void onMoveBegin(GridPosition nextPosition, CharacterDirection nextDirection);
	void movePath(float);
	bool init() override;
public:

	Unit();

	TileMap tiles;
	TileTypeMap tileTypes;
	std::unordered_set<Unit*> triggering;
	bool isInTrigger(Unit* who) const;
	
	void testTrigger(Unit* who);
	virtual void onTriggerIn(Unit* who) {}
	virtual void onTriggerOut(Unit* who) {}

	void addTile(GridPosition, TileType type = TileType::Block, cocos2d::Sprite* tile = nullptr);
	virtual void addChild(cocos2d::Node* child) override;
	virtual void addChild(cocos2d::Node* child, int z) override;
	static void addMoveListener(Unit*);
	static void removeMoveListener(Unit*);

	void tryToMove(GridPosition);
	bool tryToJump(GridPosition);
	void stopMove();

	const int SCALE;
	GridPosition getCurrentGridPosition() const;
	void setGridPosition(GridPosition newGridPosition);


	cocos2d::Vec2 getCenturalPosition();

	static Unit* create(int scale = 1);

	Unit(int scale);

	virtual void removeFromParentAndCleanup(bool) override;
	ChracterCondition getCondition() const;
	CharacterDirection getCurrentDirection();
	bool isEnemy(Unit*);

	virtual void setTarget(Unit* target);
	Unit* getTarget();
	void setMoveType(MoveType moveType);
	virtual bool isInAttackRange(Unit* who) const;
	virtual void setPosition(const cocos2d::Vec2& v) override;

	virtual void hit(Unit* by, int damage);
	virtual ~Unit();

private:
	int buffCount = 0;
public:
	virtual void buff(int power, float time, cocos2d::Node* icon);
};
