#pragma once

#include "Character.h"
#include "SkillCommand.h"
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
	std::shared_ptr<SkillCommand> command;
protected:
	bool init() override;
	virtual void onAttackBegin() override;
	virtual void onMoveBegin(GridPosition next, CharacterDirection nextDirection) override;
public:
	static Player* create(CharacterType, const int scale = 2);
	virtual void setTarget(Character* target) override;
	Weapon* getWeapon();
	std::shared_ptr<SkillCommand> getCommand();

	void buff(int, float, cocos2d::Node*) override;

	Player(CharacterType, int);

	~Player();
};
