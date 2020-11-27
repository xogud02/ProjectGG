#pragma once

#include "cocos2d.h"
#include "GaugeBar.h"
#include <unordered_map>
#include "Character.h"
#include "Skill.h"
#include "SkillIconBox.h"

using pBar = std::shared_ptr<GaugeBar>;
using CharacterBarMap = std::unordered_map<Character*, pBar>;
enum class CharacterType;

class GUILayer : public cocos2d::Node {

	static GUILayer* instance;
public:
	static GUILayer* getInstance();

private:

	CharacterBarMap characterBarMap;
	cocos2d::DrawNode* canvas = nullptr;

	SkillIconBox* blinkIconBox = nullptr;
	cocos2d::Vector<SkillIconBox*> skillIconBoxes;

	GUILayer() = default;
protected:
	virtual bool init() override;
public:
	CREATE_FUNC(GUILayer);
	void addGaugeBar(Character* owner, pBar newBar);
	void setPlayer(Character* player);
	void createBottomUI(cocos2d::Size size, CharacterType characterType);
	void addSkill(std::shared_ptr<Skill> skill);
	SkillIconBox* getBlinkIconBox();
};
