#include "ObjectSpriteFactory.h"
#include "SpriteFactory.h"

USING_NS_CC;

Action* ObjectSpriteFactory::portal() {
	return SpriteFactory::createAction("Tiles/Door", 1, 5);
}
