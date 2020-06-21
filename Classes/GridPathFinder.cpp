#include "GridPathFinder.h"
#include "Grid.h"
#include <unordered_set>
using namespace std;

queue<GridPosition> GridPathFinder::findPath(Grid* grid, GridPosition from, GridPosition to) {
	priority_queue<shared_ptr<AstarNode>> open;
	unordered_set<shared_ptr<AstarNode>> closed;
	AstarNode start(from, 0, from.distance(to));
	const float diagonal = sqrtf(2);
	int dr[] = { -1,-1,-1,0,0,1,1,1 };
	int dc[] = { -1,0,1,-1,1,-1,0,1 };
	shared_ptr<AstarNode> last;
	while (!open.empty()) {
		auto current = open.top(); open.pop();
		closed.insert(current);
		for (int i = 0; i < 8; ++i) {
			auto currentPosition = current->gridPosition;
			GridPosition adjPosition = currentPosition + GridPosition(dr[i], dc[i]);
			if (!grid->isMovable(adjPosition)) {
				continue;
			}

		}
	}

	queue<GridPosition> ret;

	return ret;
}

AstarNode::AstarNode(GridPosition gridPosition, float g, float h, shared_ptr<AstarNode> before)
	:gridPosition(gridPosition), f(g + h), g(g), h(h), before(before) {}

