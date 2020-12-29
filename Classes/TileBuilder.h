#pragma once
#include "cocos2d.h"
#include "SpriteTileEnums.h"

class GridObject;

class TileBuilder {
	TileBuilder() = default;
public:
	static cocos2d::Node* randomGround(int rows, int cols, SpriteTileTheme theme, float grassRatio = 0.3f);

	static GridObject* building(int rows, int cols, SpriteTileTheme theme);

	static GridObject* randomTestPit(int rows, int cols);
	static GridObject* randomLiquidPit(int rows, int cols, LiquidPitType);
	static GridObject* randomPit(int rows, int cols, PitContentType, PitWallType);
};
