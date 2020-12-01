#pragma once

#include "Skill.h"

class Blink : public Skill {
public:
	cocos2d::Node* getIcon();
	bool nonTarget(const cocos2d::Vec2&) override;
};
