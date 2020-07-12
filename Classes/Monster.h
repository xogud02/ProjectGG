#pragma once

#include "Character.h"

class Monster : public Character {

	Monster(float scale);
protected:
	bool initCharacter(float unitSize) override;
public:
	static Monster* create(float unitSize, float scale);
};
