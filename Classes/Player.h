#pragma once

#include "Character.h"
#include <unordered_map>

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
	std::unordered_map<CharacterDirection, cocos2d::Action*> directions;
	const CharacterType characterType;
	Weapon* weapon;
protected:
	bool init() override;
	virtual void onAttackBegin() override;
	virtual void onMoveBegin(GridPosition next, CharacterDirection nextDirection) override;
public:
	static Player* create(CharacterType, const int scale = 2);

	Player(CharacterType, int);

	~Player();
};
