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
private:
	int buffCount = 0;
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

public:
	const int SCALE;
	TileMap tiles;
	TileTypeMap tileTypes;
	std::unordered_set<Unit*> triggering;

protected:
	void releaseTarget();
	CharacterDirection getNextDirection(GridPosition nextPosition);
	virtual void onMoveBegin(GridPosition nextPosition, CharacterDirection nextDirection);
	virtual void onAttackBegin();
	void movePath(float);
	bool init() override;
public:
	static Unit* create(int scale = 1);

	Unit(int scale = 1);
	virtual ~Unit();

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

	GridPosition getCurrentGridPosition() const;
	void setGridPosition(GridPosition newGridPosition);
	virtual void setPosition(const cocos2d::Vec2& v) override;

	cocos2d::Vec2 getCenturalPosition();

	virtual void removeFromParentAndCleanup(bool) override;

	void setMoveType(MoveType moveType);
	ChracterCondition getCondition() const;
	CharacterDirection getCurrentDirection();
	bool isEnemy(Unit*);

	Unit* getTarget();
	virtual void setTarget(Unit* target);
	virtual bool isInAttackRange(Unit* who) const;
	virtual void hit(Unit* by, int damage);
	virtual void buff(int power, float time, cocos2d::Node* icon);
};
