#pragma once

#include "cocos2d.h"
#include <vector>

class Grid :public cocos2d::Ref{
	std::vector<std::vector<bool>> movableGrid;
public:
	static float UNIT_SIZE;
	Grid();
	void showGrid(cocos2d::Scene* scene);
};
