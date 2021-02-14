#pragma once
#include "cocos2d.h"
#include "SpriteTileEnums.h"

class GridUnit;

class TileBuilder {
	TileBuilder() = default;
public:
	static cocos2d::Node* randomGround(int rows, int cols, SpriteTileTheme theme, float grassRatio = 0.3f);

	static GridUnit* building(int rows, int cols, SpriteTileTheme theme);

	static GridUnit* randomTestPit(int rows, int cols);
	static GridUnit* randomLiquidPit(int rows, int cols, LiquidPitType);
	static GridUnit* randomPit(int rows, int cols, PitContentType, PitWallType);
};
