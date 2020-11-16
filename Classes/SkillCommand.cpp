#include "SkillCommand.h"

USING_NS_CC;

void SkillCommand::pullCharacter(Character *) {
	CCLOG("pull");
}

void SkillCommand::pushCharacter(Character *) {
	CCLOG("push");
}

void SkillCommand::draggingPlayer(const Vec2 &) {
	CCLOG("dragging");
}

void SkillCommand::draggedPlayer(const Vec2 &) {
	CCLOG("dragged");
}
