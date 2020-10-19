#pragma once

#include "cocos2d.h"
#include "GaugeBar.h"
#include <unordered_map>
#include "Character.h"

using pBar = std::shared_ptr<GaugeBar>;
using CharacterBarMap = std::unordered_map<Character*, pBar>;

class GUILayer : public cocos2d::Node {

	static GUILayer* instance;
public:
	static GUILayer* getInstance();

private:

	CharacterBarMap characterBarMap;
	cocos2d::DrawNode* canvas = nullptr;

	GUILayer() = default;
protected:
	virtual bool init() override;
public:
	CREATE_FUNC(GUILayer);
	void addGaugeBar(Character* owner, pBar newBar);
};
