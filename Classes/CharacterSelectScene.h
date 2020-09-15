#pragma once
#include "cocos2d.h"

enum class SpriteTileTheme;

class CharacterSelectScene : public cocos2d::Scene {
private:
	CharacterSelectScene() = default;
public:
	static CharacterSelectScene* create(SpriteTileTheme);
	virtual bool init() override;
};
