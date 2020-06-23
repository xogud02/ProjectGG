#pragma once

#include <cmath>
#include <string>

class GridPosition {
public:
	int row;
	int col;

	GridPosition(int row = 0, int col = 0) : row(row), col(col) {}

	bool operator ==(const GridPosition& rhs) const{
		return row == rhs.row && col == rhs.col;
	}

	GridPosition operator +(const GridPosition& rhs) const{
		return GridPosition(row + rhs.row, col + rhs.col);
	}

	GridPosition& operator +=(GridPosition& rhs) {
		row += rhs.row;
		col += rhs.col;
		return *this;
	}

	std::string toString() const{
		return "( " + std::to_string(row) + " , " + std::to_string(col)+ " )";
	}

	float distance(GridPosition& to) const{
		return sqrt(pow(to.row - row, 2) + pow(to.col - col, 2));
	}
};