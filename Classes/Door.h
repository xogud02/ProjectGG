#pragma once

#include "GridObject.h"
#include "SpriteTileEnums.h"

class Door : public GridObject {
	DoorLockType doorLockType;
	DoorPosition doorPosition;
	DoorType doorType;
};
