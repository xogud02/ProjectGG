#include "SkillCommand.h"
#include "GUILayer.h"
#include "GridLayer.h"

USING_NS_CC;
using namespace std;

bool SkillCommand::tryToUseSkill(CommandType command, function<bool(sptrSkill)> onUse) {
	if (!isMapped(command)) {
		return false;
	}
	bool isBlink = command == CommandType::DoubleTabGround;
	auto gui = GUILayer::getInstance();
	auto skill = skills[command];
	auto box = isBlink ? gui->getBlinkIconBox() : gui->getSkillIconBox(indice[skill]);
	if (box->isCoolingDown()) {
		return false;
	}

	if (onUse(skill)) {
		box->startCooldown();
		return true;
	}
	return false;
}

bool SkillCommand::isMapped(CommandType command) const {
	return skills.find(command) != skills.cend();
}

void SkillCommand::pullCharacter(Character* to) {
	tryToUseSkill(CommandType::PullCharacter, [to](auto skill) {return skill->onTarget(to); });
}

void SkillCommand::draggingPlayer(const Vec2& from, const Vec2& to) {
	if (!isMapped(CommandType::DragPlayer)) {
		return;
	}
	auto guiLayer = GUILayer::getInstance();
	auto gridLayer = GridLayer::getInstance();//bad smell
	guiLayer->setArrowVisible(true);
	guiLayer->setArrowPosition(gridLayer->convertToWorldSpace(from), gridLayer->convertToWorldSpace(to));
}

void SkillCommand::draggedPlayer(const Vec2 & to) {
	tryToUseSkill(CommandType::DragPlayer, [to](auto skill) {return skill->nonTarget(to); });
	GUILayer::getInstance()->setArrowVisible(false);
}

void SkillCommand::doubleTapTarget(Character * target) {
	tryToUseSkill(CommandType::DoubleTabTarget, [target](auto skill) {return skill->onTarget(target); });
}

void SkillCommand::doubleTapGround(const cocos2d::Vec2 & to) {
	tryToUseSkill(CommandType::DoubleTabGround, [to](auto skill) {return skill->nonTarget(to); });
}

void SkillCommand::addSkill(CommandType command, sptrSkill skill) {
	if (isMapped(command)) {
		CCLOG("already mapped");
		return;
	}
	int index = indice.size();
	skills[command] = skill;
	
	bool isBlink = command == CommandType::DoubleTabGround;
	
	if (!isBlink) {
		indice[skill] = index;
	}
	
	auto guiLayer = GUILayer::getInstance();
	auto box = isBlink ? guiLayer->getBlinkIconBox(): guiLayer->getSkillIconBox(index);
	box->attachIcon(skill->getIcon());
	box->setCooldown(skill->getCooldown());
}
