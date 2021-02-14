#include "Door.h"
#include "TileSpriteFactory.h"

USING_NS_CC;

Door::Door(DoorType doorType, DoorPosition doorPosition, DoorLockType doorLockType) :
	doorType(doorType),
	doorPosition(doorPosition),
	doorLockType(doorLockType) {
}

Door* Door::create(DoorPosition doorPosition, DoorType doorType, DoorLockType doorLockType) {//FIXME trigger wrong
	auto ret = new Door(doorType, doorPosition, doorLockType);
	if (!ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	auto createDoorFrame = [doorType, doorPosition, doorLockType](bool closed) {
		auto ret = TileSpriteFactory::doorFrame(doorType, doorPosition, doorLockType, closed);
		ret->retain();
		return ret;
	};

	ret->opened = createDoorFrame(false);
	ret->closed = createDoorFrame(true);
	ret->door = Sprite::createWithSpriteFrame(ret->closed);
	ret->addTile(GridPosition(), TileType::Floor, ret->door);
	GridPosition triggerPositions[2];
	int deltas[] = { -1, 1 };
	for (int i = 0; i < 2; ++i) {
		if (doorPosition == DoorPosition::Horizontal) {
			triggerPositions[i].row = deltas[i];
		} else {
			triggerPositions[i].col = deltas[i];
		}
		ret->addTile(triggerPositions[i], TileType::EventTrigger);
	}

	return ret;
}

void Door::onTriggerIn(GridUnit*) {
	door->setSpriteFrame(opened);
}

void Door::onTriggerOut(GridUnit*) {
	door->setSpriteFrame(closed);
}
