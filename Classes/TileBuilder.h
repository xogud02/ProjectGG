#pragma once
#include "cocos2d.h"
#include "SpriteTileEnums.h"

class TileBuilder {
	TileBuilder() = default;
public:
	static cocos2d::Node* randomFloor(int rows, int cols, float gridSize, SpriteTileTheme theme, float grassRatio = 0.3f);
	static cocos2d::Node* randomLiquidPit(int rows, int cols, float gridSize, LiquidPitType);
	static cocos2d::Node* randomPit(int rows, int cols, float gridSize, PitContentType, PitWallType);
};
