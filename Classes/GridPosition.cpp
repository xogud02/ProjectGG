#include <cmath>
#include "GridPosition.h"

using namespace std;

GridPosition::GridPosition(int row, int col) : row(row), col(col) {}

bool GridPosition::operator==(const GridPosition & rhs) const {
	return row == rhs.row && col == rhs.col;
}

GridPosition GridPosition::operator-(const GridPosition & rhs) const {
	return GridPosition(row - rhs.row, col - rhs.col);
}

GridPosition GridPosition::operator+(const GridPosition & rhs) const {
	return GridPosition(row + rhs.row, col + rhs.col);
}

GridPosition & GridPosition::operator+=(GridPosition & rhs) {
	row += rhs.row;
	col += rhs.col;
	return *this;
}

string GridPosition::toString() const {
	return "( " + std::to_string(row) + " , " + std::to_string(col) + " )";
}

float GridPosition::distance(GridPosition & to) const {
	return sqrtf(powf(to.row - row, 2) + powf(to.col - col, 2));
}

namespace std {
	size_t hash<GridPosition>::operator()(const GridPosition& g) const{
		return hash<int>()(g.row * 1000 + g.col);
	}
}
