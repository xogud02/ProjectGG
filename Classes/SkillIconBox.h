#pragma once

#include "cocos2d.h"

class GUIBoxCreator;
class SkillIconBox : public cocos2d::Node {
	SkillIconBox() = default;
	cocos2d::DrawNode* draw = nullptr;
	cocos2d::Label* txt = nullptr;
	float cooldown = 3.f;
public:
	static SkillIconBox* create(GUIBoxCreator preset, cocos2d::Node* icon);
	virtual bool init() override;
	void setCooldown(float cooldown);
	void startCooldown();
	bool isCoolingDown();
};
