#pragma once

#include "cocos2d.h"
#include <unordered_set>

class GridUnit;
class GridUnits : public  cocos2d::Node{
	std::unordered_set<GridUnit*> units;
public:
	void addUnit(GridUnit*);

};