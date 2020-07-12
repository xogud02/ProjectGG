#pragma once

#include "Character.h"
#include <vector>

class Player : public Character {
	std::vector<std::pair<std::function<bool(float)>, cocos2d::Action*>> directions;
	cocos2d::Action* currentAction;
public:
	static Player* create(const float unitSize, const int scale = 2);
	virtual void moveTo(cocos2d::Vec2) override;
	Player(int scale);
	~Player();
};