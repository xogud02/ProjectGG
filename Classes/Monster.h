#pragma once

#include "Unit.h"

class Monster : public Unit {

	Monster(int scale);
protected:
	bool init() override;
	virtual void onMoveBegin(GridPosition, CharacterDirection nextDirection);
public:
	static Monster* create(float scale);
};
