#include "GridUnits.h"

void GridUnits::addUnit(GridUnit *unit) {
	units.insert(unit);
}

UnitSet GridUnits::getUnits() const {
	return units;
}
