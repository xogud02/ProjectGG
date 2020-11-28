#include "SkillCommand.h"
#include "GUILayer.h"

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

void SkillCommand::doubleTapTarget(Character * target) {
	if (!isMapped(CommandType::DoubleTabTarget)) {
		return;
	}
	auto skill = skills[CommandType::DoubleTabTarget];
	auto box = GUILayer::getInstance()->getSkillIconBox(indice[skill]);
	if (box->isCoolingDown()) {
		return;
	}
	skill->onTarget(target);
	box->startCooldown();
	CCLOG("double tap target");
}

void SkillCommand::addSkill(CommandType command, std::shared_ptr<Skill> skill) {
	if (isMapped(command)) {
		CCLOG("already mapped");
		return;
	}
	int index = skills.size();
	skills[command] = skill;
	indice[skill] = index;
	auto box = GUILayer::getInstance()->getSkillIconBox(index);
	box->attachIcon(skill->getIcon());
	box->setCooldown(skill->getCooldown());
}
