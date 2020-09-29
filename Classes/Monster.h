#pragma once

#include "Character.h"

class Monster : public Character {

	Monster(int scale);
protected:
	bool init() override;
	virtual void onMoveBegin(GridPosition, CharacterDirection nextDirection);
public:
	static Monster* create(float scale);
};
