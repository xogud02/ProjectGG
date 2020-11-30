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

class Character;

using sptrSkill = std::shared_ptr<Skill>;
class SkillCommand {
	std::unordered_map<CommandType, sptrSkill > skills;
	std::unordered_map<sptrSkill, int> indice;
	void tryToUseSkill(CommandType, std::function<void(sptrSkill)>);
public:
	bool isMapped(CommandType) const;
	void pullCharacter(Character*);
	void draggingPlayer(const cocos2d::Vec2&);
	void draggedPlayer(const cocos2d::Vec2&);
	void doubleTapTarget(Character*);
	void doubleTapGround(const cocos2d::Vec2&);

	void addSkill(CommandType, sptrSkill);
};
