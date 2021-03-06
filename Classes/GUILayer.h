#pragma once

#include "cocos2d.h"
#include "GaugeBar.h"
#include <unordered_map>
#include "Player.h"
#include "Skill.h"
#include "SkillIconBox.h"
#include "GUIBoxCreator.h"

using pBar = std::shared_ptr<GaugeBar>;
using CharacterBarMap = std::unordered_map<GridUnit*, pBar>;
enum class CharacterType;

class GUILayer : public cocos2d::Node {

	static GUILayer* instance;
public:
	static GUILayer* getInstance();
	CREATE_FUNC(GUILayer);

private:

	GUILayer();
	GUIBoxCreator theme;
	cocos2d::Size bottomUISize;
	cocos2d::Size iconSize;

	CharacterBarMap characterBarMap;
	cocos2d::DrawNode* canvas = nullptr;

	SkillIconBox* blinkIconBox = nullptr;
	cocos2d::Vector<SkillIconBox*> skillIconBoxes;

	//std::queue<SkillIconBox*> buffs;

	cocos2d::Sprite* arrow = nullptr;


protected:
	virtual bool init() override;
public:

	void addGaugeBar(GridUnit* owner, pBar newBar);
	void setPlayer(Player* player);
	void createBottomUI(cocos2d::Size size, CharacterType characterType);
	void setArrowPosition(const cocos2d::Vec2& from, const cocos2d::Vec2& to);
	void setArrowVisible(bool);
	void setArrowTransParent(bool);
	SkillIconBox* getSkillIconBox(int);
	SkillIconBox* getBlinkIconBox();

	void addBuff(cocos2d::Node* icon, float duration);
};
