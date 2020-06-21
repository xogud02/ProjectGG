#pragma once

#include <cmath>

class GridPosition {
public:
	int row;
	int col;

	GridPosition(int row = 0, int col = 0) : row(row), col(col) {}

	bool operator ==(GridPosition& rhs) {
		return row == rhs.row && col == rhs.col;
	}

	GridPosition operator +(GridPosition& rhs) {
		return GridPosition(row + rhs.row, col + rhs.col);
	}

	GridPosition& operator +=(GridPosition& rhs) {
		row += rhs.row;
		col += rhs.col;
		return *this;
	}

	float distance(GridPosition& to) {
		return sqrt(pow(to.row - row, 2) + pow(to.col - col, 2));
	}
};