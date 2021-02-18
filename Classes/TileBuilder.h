#pragma once
#include "cocos2d.h"
#include "SpriteTileEnums.h"

class GridUnits;

class TileBuilder {
	TileBuilder() = default;
public:
	static cocos2d::Node* randomGround(int rows, int cols, SpriteTileTheme theme, float grassRatio = 0.3f);

	static GridUnits* building(int rows, int cols, SpriteTileTheme theme);

	static GridUnits* randomTestPit(int rows, int cols);
	static GridUnits* randomLiquidPit(int rows, int cols, LiquidPitType);
	static GridUnits* randomPit(int rows, int cols, PitContentType, PitWallType);
};
