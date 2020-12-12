#pragma once
#include "Skill.h"

class PowerUp : public Skill {
public:
	bool onTarget(Character*) override;
};
