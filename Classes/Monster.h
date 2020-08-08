#pragma once

#include "Character.h"

class Monster : public Character {

	Monster(int scale);
protected:
	bool init() override;
public:
	static Monster* create(float scale);
	void moveRandom(float dt);
};
