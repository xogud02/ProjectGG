#pragma once

#include "cocos2d.h"
#include "GridObject.h"
#include "SpriteTileEnums.h"

class Door : public GridObject {
	DoorLockType doorLockType = DoorLockType::None;
	DoorPosition doorPosition = DoorPosition::Horizontal;
	DoorType doorType = DoorType::Wood;
	cocos2d::Sprite* opened = nullptr;
	cocos2d::Sprite* closed = nullptr;
};
