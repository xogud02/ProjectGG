#pragma once
#include "cocos2d.h"

class TileBuilder {
	TileBuilder() = default;
public:
	static cocos2d::Node* randomFloor(int row, int cols, float gridSize, float grassRatio = 0.3f);
};
