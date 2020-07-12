#pragma once

#include "Character.h"

class Monster : public Character {

	Monster(float scale);

public:
	static Monster* create(float unitSize, float scale);
};
