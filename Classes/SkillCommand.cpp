#include "SkillCommand.h"

USING_NS_CC;
using namespace std;

bool SkillCommand::isMapped(CommandType command) const {
	return skills.find(command) != skills.cend();
}

void SkillCommand::pullCharacter(Character* to) {
	if (!isMapped(CommandType::PullCharacter)) {
		return;
	}
	skills[CommandType::PullCharacter]->onTarget(to);
	CCLOG("pull");
}

void SkillCommand::pushCharacter(Character* to) {
	if (!isMapped(CommandType::PushCharacter)) {
		return;
	}
	skills[CommandType::PushCharacter]->onTarget(to);
	CCLOG("push");
}

void SkillCommand::draggingPlayer(const Vec2 &) {
	if (!isMapped(CommandType::DragPlayer)) {
		return;
	}
	CCLOG("dragging");
}

void SkillCommand::draggedPlayer(const Vec2 & to) {
	if (!isMapped(CommandType::DragPlayer)) {
		return;
	}
	skills[CommandType::DragPlayer]->nonTarget(to);
	CCLOG("dragged");
}

void SkillCommand::addSkill(CommandType command, std::shared_ptr<Skill> skill) {
	skills[command] = skill;
}
