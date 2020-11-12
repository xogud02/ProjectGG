#include "GridTouchListener.h"
#include "GridLayer.h"
#include "Player.h"

USING_NS_CC;
using namespace std;

GridTouchListener * GridTouchListener::create(Node* owner) {
	auto ret = new GridTouchListener();
	if (!ret || !ret->init()) {
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	ret->owner = owner;
	ret->EventListenerTouchOneByOne::onTouchBegan = CC_CALLBACK_2(GridTouchListener::onTouchBegan, ret);
	ret->EventListenerTouchOneByOne::onTouchEnded = CC_CALLBACK_2(GridTouchListener::onTouchEnded, ret);
	ret->EventListenerTouchOneByOne::onTouchMoved = CC_CALLBACK_2(GridTouchListener::onTouchMoved, ret);
	owner->getEventDispatcher()->addEventListenerWithSceneGraphPriority(ret, owner);
	return ret;
}

bool GridTouchListener::onTouchBegan(Touch* t, Event *) {
	const auto key = "doubleTab interval";
	auto currentTouched = t->getLocation();
	if (lastTouched == invalidPosition || lastTouched.distance(currentTouched) > 1.5f) {
		lastTouched = currentTouched;
		owner->scheduleOnce([this](float)mutable {lastTouched = invalidPosition; }, 0.3f, key);
		if (onSingleTouch) {
			onSingleTouch(currentTouched);
		}
	} else {
		if (owner->isScheduled(key)) {
			owner->unschedule(key);
			if (onDoubleTouch) {
				onDoubleTouch(currentTouched);
			}
		}
		lastTouched = invalidPosition;
		return true;
	}
	return true;
}

bool GridTouchListener::onTouchEnded(Touch *, Event *) {
	return false;
}

bool GridTouchListener::onTouchMoved(Touch *, Event *) {
	return false;
}
