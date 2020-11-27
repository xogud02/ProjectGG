#pragma once

#include "cocos2d.h"

class GUIBoxCreator;
class SkillIconBox : public cocos2d::Node {
	SkillIconBox() = default;
	cocos2d::DrawNode* draw = nullptr;
	cocos2d::Label* label = nullptr;
	cocos2d::Node* icon = nullptr;
	float iconSize;
	float innerIconSize;
	float cooldown = 3.f;
public:
	static SkillIconBox* create(GUIBoxCreator preset);
	virtual bool init() override;
	void attachIcon(Node* icon);
	void setCooldown(float cooldown);
	void startCooldown();
	bool isCoolingDown();
};
