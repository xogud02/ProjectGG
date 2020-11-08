#include "Skill.h"
#include "Character.h"

Skill::Skill(float coolDown, OnSkill onSkill):
	coolDown(coolDown),
	skillAction(onSkill)
{}

void Skill::useSkill(Character * by) {
	skillAction(by);
}
