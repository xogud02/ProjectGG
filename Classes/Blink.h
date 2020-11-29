#pragma once

#include "Skill.h"

class Blink : public Skill {
public:
	cocos2d::Node* getIcon();
	void nonTarget(const cocos2d::Vec2&) override;
};
