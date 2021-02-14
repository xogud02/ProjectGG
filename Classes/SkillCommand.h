#pragma once

#include "cocos2d.h"
#include "Skill.h"
#include <unordered_map>
#include <functional>

enum class CommandType {
	PullCharacter,
	DragPlayer,
	DoubleTabTarget,
	DoubleTabGround
};

class GridUnit;

using sptrSkill = std::shared_ptr<Skill>;
class SkillCommand {
	std::unordered_map<CommandType, sptrSkill > skills;
	std::unordered_map<sptrSkill, int> indice;
	bool tryToUseSkill(CommandType, std::function<bool(sptrSkill)>);
public:
	bool isMapped(CommandType) const;
	void pullCharacter(GridUnit*);
	void draggingPlayer(const cocos2d::Vec2&, const cocos2d::Vec2&);
	void draggedPlayer(const cocos2d::Vec2&);
	void doubleTapTarget(GridUnit*);
	void doubleTapGround(const cocos2d::Vec2&);

	void addSkill(CommandType, sptrSkill);
};
