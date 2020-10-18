#include "SpriteFactory.h"
#include "Player.h"
using namespace std;
USING_NS_CC;

const string SpriteFactory::EXT = ".png";
const string SpriteFactory::root = "Sprites/";

const Size SpriteFactory::unitSize(SpriteFactory::iUnitSize, SpriteFactory::iUnitSize);

Action* SpriteFactory::createAction(const string& path, int x, int y) {
	return createAnim([path, x, y](int i) {return createFrame(path + to_string(i), x, y); });
}

Action* SpriteFactory::createAnim(function<SpriteFrame*(char)>createFrame) {
	Vector<SpriteFrame*> frames;
	for (int i = 0; i < 2; ++i) {
		frames.pushBack(createFrame(i));
	}
	auto animation = Animation::createWithSpriteFrames(frames, 0.3f);
	auto singleLoop = Animate::create(animation);
	return RepeatForever::create(singleLoop);
}

float SpriteFactory::getUnitScale(float sizeInPoints) {
	return sizeInPoints * CC_CONTENT_SCALE_FACTOR() / iUnitSize;
}

Size SpriteFactory::getUnitSizeInPoints() {
	return CC_SIZE_PIXELS_TO_POINTS(unitSize);
}


bool flag = true;

SpriteFrame* SpriteFactory::createFrame(const string& fileName, int x, int y) {
	auto xOffset = x * iUnitSize, yOffset = y * iUnitSize;
	auto ret = SpriteFrame::create(root + fileName + EXT, CC_RECT_PIXELS_TO_POINTS(Rect(x * iUnitSize, y*iUnitSize, iUnitSize, iUnitSize)));
	ret->getTexture()->setAliasTexParameters();
	return ret;
}

