#pragma once

#include "cocos2d.h"
#include "Unit.h"
#include "SpriteTileEnums.h"

class Door : public Unit {
	DoorType doorType = DoorType::Wood;
	DoorPosition doorPosition = DoorPosition::Horizontal;
	DoorLockType doorLockType;
	cocos2d::Sprite* door;
	cocos2d::SpriteFrame* opened = nullptr;
	cocos2d::SpriteFrame* closed = nullptr;

	Door(DoorType doorType, DoorPosition doorPosition, DoorLockType doorLockType);

	
public:
	static Door* create(DoorPosition doorPosition = DoorPosition::Horizontal, DoorType doorType = DoorType::Wood, DoorLockType doorLockType = DoorLockType::None);
	virtual void onTriggerIn(Unit* who) override;
	virtual void onTriggerOut(Unit* who) override;
};
