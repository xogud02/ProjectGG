#pragma once

#include <queue>
#include <memory>
#include "GridPosition.h"
#include <string>

class Grid;

class AstarNode;
using SPNode = std::shared_ptr<AstarNode>;

struct AstarNode {
	const GridPosition gridPosition;
	const float f;
	const float g;
	const float h;
	SPNode parent;

	AstarNode(GridPosition gridPosition, float g, float h, SPNode before = nullptr);
	
	std::string toString() {
		return gridPosition.toString() + "g : "+ std::to_string(g)+ ", h : " + std::to_string(h) + ", f : "+std::to_string(f);
	}
};

class GridPathFinder{
	
public:
	std::queue<GridPosition> findPath(Grid* grid, GridPosition from, GridPosition to,int size = 1);
};
