#pragma once
#include "cocos2d.h"

enum class SpriteTileTheme;
class TileBuilder {
	TileBuilder() = default;
public:
	static cocos2d::Node* randomFloor(int row, int cols, float gridSize, SpriteTileTheme theme, float grassRatio = 0.3f);
};
