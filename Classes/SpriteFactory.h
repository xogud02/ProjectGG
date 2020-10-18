#pragma once
#include "cocos2d.h"

class SpriteFactory {
	SpriteFactory() = default;
	static const int iUnitSize = 16;

public:
	static const std::string root;
	static const std::string EXT;
	static const cocos2d::Size unitSize;

	static cocos2d::SpriteFrame* createFrame(const std::string& fileName, int x, int y);
	static cocos2d::Action* createAction(const std::string& path, int x, int y);
	static cocos2d::Action* createAnim(std::function<cocos2d::SpriteFrame*(char)> createFrame);

	static float getUnitScale(float sizeInPoints);

	static cocos2d::Size getUnitSizeInPoints();

};

