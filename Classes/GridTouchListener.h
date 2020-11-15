#pragma once

#include "cocos-ext.h"
#include <functional>

class GridTouchListener : protected cocos2d::EventListenerTouchOneByOne {
public:
	static GridTouchListener* create(cocos2d::Node* owner);
private:
	cocos2d::Node* owner;
	const cocos2d::Vec2 invalidPosition = cocos2d::Vec2(-1, -1);
	cocos2d::Vec2 holdBeginPosition = invalidPosition;
	cocos2d::Vec2 lastTouched = invalidPosition;


	bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	bool onTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	bool onTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	
public:
	float dragThreshold = 1.5f;
	std::function<void(cocos2d::Vec2)> onSingleTouch = nullptr;
	std::function<void(cocos2d::Vec2)> onDoubleTouch = nullptr;
	std::function<void(cocos2d::Vec2, cocos2d::Vec2)> onDrag = nullptr;
	std::function<void(cocos2d::Vec2, cocos2d::Vec2)> onDragEnded = nullptr;
};
