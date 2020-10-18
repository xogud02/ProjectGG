#pragma once

#include "cocos2d.h"
#include "SpriteTileEnums.h"

class GUISpriteFactory {
	static const std::string GUI;

public:
	static cocos2d::SpriteFrame* GUIArrowFrame(ArrowDirection);

	static cocos2d::SpriteFrame* GUIFrame(GUIFrameColor, GUIFramePart, bool filled);

};
