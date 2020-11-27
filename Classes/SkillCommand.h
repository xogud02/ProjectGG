#pragma once

#include "cocos2d.h"
#include "Skill.h"
#include <unordered_map>

enum class CommandType {
	PullCharacter,
	DragPlayer,
	DoubleTabTarget
};

class Character;

class SkillCommand {
	std::unordered_map<CommandType, std::shared_ptr<Skill>> skills;
public:
	bool isMapped(CommandType) const;
	void pullCharacter(Character*);
	void draggingPlayer(const cocos2d::Vec2&);
	void draggedPlayer(const cocos2d::Vec2&);
	void doubleTapTarget(Character*);

	void addSkill(CommandType, std::shared_ptr<Skill>);
};
