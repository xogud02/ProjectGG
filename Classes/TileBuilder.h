#pragma once
#include "cocos2d.h"
#include "SpriteTileEnums.h"

class Unit;

class TileBuilder {
	TileBuilder() = default;
public:
	static cocos2d::Node* randomGround(int rows, int cols, SpriteTileTheme theme, float grassRatio = 0.3f);

	static Unit* building(int rows, int cols, SpriteTileTheme theme);

	static Unit* randomTestPit(int rows, int cols);
	static Unit* randomLiquidPit(int rows, int cols, LiquidPitType);
	static Unit* randomPit(int rows, int cols, PitContentType, PitWallType);
};
