#pragma once

#include "cocos2d.h"
#include <unordered_set>

class GridUnit;
using UnitSet = std::unordered_set<GridUnit*>;
class GridUnits : public  cocos2d::Node{
	UnitSet units;
public:
	void addUnit(GridUnit*);
	UnitSet getUnits() const;
};