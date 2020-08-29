#pragma once

#include "Character.h"
#include <vector>

class Weapon;

enum class CharacterType {
	Engineer,
	Mage,
	Paladin,
	Rogue,
	Template,
	Warrior
};

constexpr int CharacterTypeCount = static_cast<int>(CharacterType::Warrior) + 1;

class Player : public Character {
	std::vector<std::pair<std::function<bool(float)>, cocos2d::Action*>> directions;
	const CharacterType characterType;
	cocos2d::Action* currentAction;
	Weapon* weapon;
protected:
	bool init() override;
	virtual void onMoveBegin(GridPosition next) override;
public:
	static Player* create(CharacterType, const int scale = 2);

	virtual AttackResult attack(Character* c) override;
	Player(CharacterType, int);

	~Player();
};
