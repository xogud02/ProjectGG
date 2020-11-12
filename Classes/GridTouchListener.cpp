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
	holdBeginPosition = owner->convertTouchToNodeSpace(t);
	return true;
}

bool GridTouchListener::onTouchEnded(Touch* t, Event *) {
	auto touchEnded = owner->convertTouchToNodeSpace(t);
	if (holdBeginPosition.distance(touchEnded) > dragThreshold) {
		if (onDragEnded) {
			onDragEnded(holdBeginPosition, touchEnded);
			holdBeginPosition = invalidPosition;
			return true;
		}
	}

	const auto key = "doubleTab interval";
	if (lastTouched == invalidPosition || lastTouched.distance(touchEnded) > dragThreshold) {
		lastTouched = touchEnded;
		owner->scheduleOnce([this](float)mutable {lastTouched = invalidPosition; }, 0.3f, key);
		if (onSingleTouch) {
			onSingleTouch(touchEnded);
		}
	} else {
		if (owner->isScheduled(key)) {
			owner->unschedule(key);
			if (onDoubleTouch) {
				onDoubleTouch(touchEnded);
			}
		}
		lastTouched = invalidPosition;
		return true;
	}
	return true;
}

bool GridTouchListener::onTouchMoved(Touch* t, Event *) {
	if (onDrag) {
		onDrag(holdBeginPosition, owner->convertTouchToNodeSpace(t));
	}

	return true;
}
