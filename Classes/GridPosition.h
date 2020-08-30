#pragma once

#include <string>

class GridPosition {
public:
	int row;
	int col;

	GridPosition(int row = 0, int col = 0);

	bool operator ==(const GridPosition& rhs) const;
	GridPosition operator -(const GridPosition& rhs) const;

	GridPosition operator +(const GridPosition& rhs) const;

	GridPosition& operator +=(GridPosition& rhs);

	std::string toString() const;

	float distance(GridPosition& to) const;
};

namespace std {
	template<>
	struct hash<GridPosition> {
		size_t operator()(const GridPosition& g) const;
	};
}
