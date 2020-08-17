#pragma once

#include "Character.h"
#include <vector>

class Weapon;
class Player : public Character {
	std::vector<std::pair<std::function<bool(float)>, cocos2d::Action*>> directions;
	cocos2d::Action* currentAction;
	Weapon* weapon;
protected:
	bool init() override;
	virtual void onMoveBegin(GridPosition next) override;
public:
	static Player* create(const int scale = 2);

	virtual AttackResult attack(Character* c) override;
	Player(int scale);

	~Player();
};
