#pragma once

#include "cocos2d.h"
#include <vector>

class Grid :public cocos2d::Ref{
	std::vector<std::vector<bool>> movableGrid;
public:
	const float UNIT_SIZE;
	Grid(float unitSize);
	void showGrid(cocos2d::Scene* scene);
	cocos2d::Sprite * createGridSquare();
};
