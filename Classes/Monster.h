#pragma once

#include "GridUnit.h"

class Monster : public GridUnit {

	Monster(int scale);
protected:
	bool init() override;
	virtual void onMoveBegin(GridPosition, CharacterDirection nextDirection);
public:
	static Monster* create(float scale);
};
