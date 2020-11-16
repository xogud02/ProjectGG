#pragma once

#include "cocos2d.h";

enum class CommandType {

};

class Character;

class SkillCommand {
public:
	void pullCharacter(Character*);
	void pushCharacter(Character*);
	void draggingPlayer(const cocos2d::Vec2&);
	void draggedPlayer(const cocos2d::Vec2&);
};
