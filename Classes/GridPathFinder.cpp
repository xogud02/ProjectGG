#include "GridPathFinder.h"
#include "Grid.h"
#include <unordered_set>
using namespace std;


namespace std {
	template <>
	struct hash<SPNode> {
		size_t operator()(const SPNode& val) const {
			return hash<GridPosition>()(val->gridPosition);
		}
	};
	
	template<>
	struct greater<SPNode> {
		bool operator()(const SPNode& lhs, const SPNode& rhs) const {
			return lhs->f > rhs->f;
		}
	};

	template<>
	struct equal_to<SPNode> {
		bool operator()(const SPNode& lhs, const SPNode& rhs) const {
			return lhs->gridPosition == rhs->gridPosition;
		}
	};
}

queue<GridPosition> GridPathFinder::findPath(Grid* grid, GridPosition from, GridPosition to,int size) {
	priority_queue<SPNode,vector<SPNode>,greater<SPNode>> pq;
	unordered_set<SPNode> open;
	unordered_set<SPNode> closed;

	auto start = make_shared<AstarNode>(from, 0, from.distance(to));
	const float diagonal = sqrtf(2);
	int dr[] = { -1,-1,-1,0,0,1,1,1 };
	int dc[] = { -1,0,1,-1,1,-1,0,1 };
	pq.push(start);
	open.insert(start);


	SPNode last;
	while (!pq.empty()) {
		auto current = pq.top(); pq.pop();
		open.erase(current);
		closed.insert(current);
		auto currentPosition = current->gridPosition;
		if (currentPosition == to) {
			last = current;

			break;
		}
		for (int i = 0; i < 8; ++i) {
			GridPosition adjPosition = currentPosition + GridPosition(dr[i], dc[i]);
			
			if (!grid->isMovable(adjPosition, size)) {
				continue;
			}

			float nextG = current->g + (dr[i] * dc[i] == 0 ? 1 : diagonal);
			auto next = make_shared<AstarNode>(adjPosition, nextG, adjPosition.distance(to), current);
			if (closed.find(next) != closed.end()) {
				continue;
			}

			auto result = open.insert(next);
			if (result.second) {
				pq.push(next);
			}
			else {
				auto beforeNext = result.first;
				if ((*beforeNext)->g > nextG) {
					open.erase(beforeNext);
					open.insert(next);
					pq.push(next);
				}
			}
		}
		
	}

	queue<GridPosition> ret;


	if (last) {
		stack<GridPosition> s;
		while (last) {
			s.push(last->gridPosition);
			last = last->parent;
		}
		while (!s.empty()) {
			ret.push(s.top());
			s.pop();
		}
 	}

	return ret;
}

AstarNode::AstarNode(GridPosition gridPosition, float g, float h, SPNode before)
	:gridPosition(gridPosition), f(g + h), g(g), h(h), parent(before) {}

string AstarNode::toString() {
	return gridPosition.toString() + "g : " + to_string(g) + ", h : " + to_string(h) + ", f : " + to_string(f);
}

