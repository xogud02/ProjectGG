#include "Door.h"
#include "TileSpriteFactory.h"

USING_NS_CC;

Door::Door(DoorType doorType, DoorPosition doorPosition, DoorLockType doorLockType):
	doorType(doorType),
	doorPosition(doorPosition),
	doorLockType(doorLockType){}

Door* Door::create(DoorPosition doorPosition, DoorType doorType, DoorLockType doorLockType) {
	auto ret = new Door(doorType, doorPosition, doorLockType);
	if (!ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	auto createDoorFrame = [doorType, doorPosition, doorLockType](bool closed) {
		auto ret = TileSpriteFactory::doorFrame(doorType, doorPosition, doorLockType, false);
		ret->retain();
		return ret;
	};

	ret->opened = createDoorFrame(false);
	ret->closed = createDoorFrame(true);
	ret->door = Sprite::createWithSpriteFrame(ret->closed);
	ret->onTriggerIn = [ret](auto c) {ret->door->setSpriteFrame(ret->opened); };
	ret->onTriggerOut = [ret](auto c) {ret->door->setSpriteFrame(ret->closed); };
	ret->addTile(GridPosition(), TileType::Floor, ret->door);

	return ret;
}
