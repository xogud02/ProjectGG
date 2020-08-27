#pragma once
#include "cocos2d.h"

class CharacterSelectScene : public cocos2d::Scene {
private:
	CharacterSelectScene() = default;
public:
	CREATE_FUNC(CharacterSelectScene);
	virtual bool init() override;
};
