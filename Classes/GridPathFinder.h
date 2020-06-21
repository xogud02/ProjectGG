#pragma once

#include <queue>
#include <memory>
#include "GridPosition.h"

class Grid;

struct AstarNode {
	const GridPosition gridPosition;
	const float f;
	const float g;
	const float h;
	std::shared_ptr<AstarNode> before;

	AstarNode(GridPosition gridPosition, float g, float h, std::shared_ptr<AstarNode> before = nullptr);

};

class GridPathFinder{
	
public:
	std::queue<GridPosition> findPath(Grid* grid, GridPosition from, GridPosition to);
};
