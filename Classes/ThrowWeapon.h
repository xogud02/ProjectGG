#pragma once

#include "Skill.h"

class ThrowWeapon : public Skill {
public:
	ThrowWeapon(float coolDown = 5);
	void onTarget(Character* to) override;
};
