#pragma once

#include "Skill.h"

class FireBall : public Skill {
public:
	cocos2d::Node* getIcon() override;
	bool nonTarget(const cocos2d::Vec2&) override;
};
