#pragma once

#include <queue>
#include <memory>
#include <string>
#include "GridPosition.h"

class GridLayer;

struct AstarNode;
using SPNode = std::shared_ptr<AstarNode>;

struct AstarNode {
	const GridPosition gridPosition;
	const float f;
	const float g;
	const float h;
	SPNode parent;

	AstarNode(GridPosition gridPosition, float g, float h, SPNode before = nullptr);
	
	std::string toString();
};

class GridPathFinder{
	
public:
	std::queue<GridPosition> findPath(GridLayer* grid, GridPosition from, GridPosition to,int size = 1);
};
