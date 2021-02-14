#pragma once

#include "Skill.h"

class ThrowWeapon : public Skill {
public:
	ThrowWeapon(float coolDown = 5);
	bool onTarget(GridUnit* to) override;
};
